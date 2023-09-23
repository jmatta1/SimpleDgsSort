#!/usr/local/bin/python3
"""This script will produce a template file or files of the appropriate types
for the selected option"""
import sys
import os
import argparse
import subprocess
import datetime
import errno
from itertools import groupby

TEMP_PARAMS = {}
exec(open("mt_cfg.py").read(), TEMP_PARAMS)
CONFIG = TEMP_PARAMS["CONFIG"]
TEMP_PARAMS = {}
exec(open("mt_templates.py").read(), TEMP_PARAMS)
TEMPLATES = TEMP_PARAMS["TEMPLATES"]


def main():
    """Set up the argument parser, then call the main function"""
    # Make the argument parser
    arg_parser = argparse.ArgumentParser(description=PARSER_DESCRIPTION)
    # Optional argument for the directory to put the generated files in
    arg_parser.add_argument("-d", nargs=1, type=str, action="store",
                            metavar=("GenDir"), help=DIR_HELP, default=".")
    # Add argument to set namespace for contents of file
    arg_parser.add_argument('-n', nargs=1, type=str, action="store",
                            metavar=("NamespaceList"), help=NS_HELP)
    # group of arguments for setting the mode of the generation
    mode_group = arg_parser.add_mutually_exclusive_group(required=True)
    # make a template class set of files
    mode_group.add_argument("-t", nargs='+', type=str, action="store",
                            metavar=("ClassName", "TemplateParameters"),
                            help=TC_HELP)
    # make a regular class set of files
    mode_group.add_argument("-c", nargs=1, type=str, action="store",
                            metavar=("ClassName"), help=C_HELP)
    # make a header only class
    mode_group.add_argument("-hc", nargs=1, type=str, action="store",
                            metavar=("ClassName"), help=HC_HELP)
    # make an abstract base class
    mode_group.add_argument("-a", nargs=1, type=str, action="store",
                            metavar=("ClassName"), help=A_HELP)
    # make a singleton class
    mode_group.add_argument("-s", nargs=1, type=str, action="store",
                            metavar=("ClassName"), help=S_HELP)
    # make a .h and .cpp for holding libraries of functions
    mode_group.add_argument("-l", nargs=1, type=str, action="store",
                            metavar=("LibraryName"), help=L_HELP)
    # make a factory for a whole sub classes of a base class system
    mode_group.add_argument("-f", nargs=2, type=str, action="store",
                            metavar=("FactoryType", "BaseClass"),
                            help=FACTORY_HELP)
    # make a registry to retrieve constructed objects derived from a given
    # subclass
    mode_group.add_argument("-r", nargs=2, type=str, action="store",
                            metavar=("RegistryType", "BaseClass"),
                            help=REG_HELP)
    # make a barebones project
    mode_group.add_argument("-p", dest="create_project",
                            action='store_true', help=PROJ_HELP)
    # make just a header file
    mode_group.add_argument("-hdr", nargs=1, type=str, action="store",
                            metavar=("HeaderName"), help=HF_HELP)
    # make just a cpp file
    mode_group.add_argument("-src", nargs=1, type=str, action="store",
                            metavar=("SourceName"), help=CF_HELP)
    # Add an option to disable updating cmake files
    arg_parser.add_argument('--skip-cmake', dest="edit_cmake",
                            action='store_false', help=SKIP_CMAKE_HELP)
    # Add argument to disable automatically adding the files to git
    arg_parser.add_argument('--skip-git', dest="add_to_git",
                            action='store_false', help=SKIP_GIT_HELP)
    # Add argument to overwrite files
    arg_parser.add_argument('--overwrite', dest="allow_overwrite",
                            action='store_true', help=OVERWRITE_HELP)
    args = arg_parser.parse_args()
    # run the program itself
    create_template(args, arg_parser)


def create_template(args, arg_parser):
    """figure out the generation option needed, then call the appropriate gen
    function"""
    # normalize the directory argument
    if not args.create_project:
        norm_dir = normalize_and_check_dir(args.d[0])
    elif args.d is not None and args.d != ".":
        norm_dir = ""
        print("Ignoring directory option for project creation.")
    # create the variable to hold the generated files
    gen_files = None
    # caldulate the format disctionary for the file
    today = datetime.datetime.today()
    fmt_dict = CONFIG
    fmt_dict["project_name_cap"] = CONFIG["project_name"].upper()
    fmt_dict["year"] = str(today.year)
    fmt_dict["day"] = str(today.day)
    fmt_dict["month_abbrev"] = today.strftime("%b")
    CONFIG["Allow Overwrite"] = False
    if args.allow_overwrite:
        CONFIG["Allow Overwrite"] = True

    if args.n is not None: # test for set namespaces
        op, cl = calculate_namespace_strings(args.n[0])
        fmt_dict["namespace_open"], fmt_dict["namespace_close"] = op, cl
        fmt_dict["namespace_name"] = args.n[0]
    else:
        fmt_dict["namespace_open"] = "\n"
        fmt_dict["namespace_close"] = "\n"
    # run through some if statements to work out what generation option was
    # actually chosen
    if args.t is not None:  # test for template class
        fmt_dict["class_name"] = args.t[0]
        fmt_dict["template_param_list"] = args.t[1:]
        gen_files = generate_template_class(norm_dir, fmt_dict)
    elif args.c is not None:  # test for class
        fmt_dict["class_name"] = args.c[0]
        gen_files = generate_class(norm_dir, fmt_dict)
    elif args.hc is not None:  # test for header only class
        fmt_dict["class_name"] = args.hc[0]
        gen_files = generate_header_class(norm_dir, fmt_dict)
    elif args.a is not None:  # test for class
        fmt_dict["class_name"] = args.a[0]
        gen_files = generate_abc(norm_dir, fmt_dict)
    elif args.f is not None:
        if args.n is None:
            print("\n`-n` option must be specified with the factory option `-f`\n")
            arg_parser.print_help()
            sys.exit()
        fmt_dict["factory_type"] = args.f[0]
        fmt_dict["base_class"] = args.f[1]
        gen_files = generate_factory(norm_dir, fmt_dict)
    elif args.s is not None:
        fmt_dict["class_name"] = args.s[0]
        gen_files = generate_singleton(norm_dir, fmt_dict)
    elif args.r is not None:
        fmt_dict["registry_type"] = args.r[0]
        fmt_dict["base_class"] = args.r[1]
        gen_files = generate_registry(norm_dir, fmt_dict)
    elif args.l is not None:  # test for library
        fmt_dict["lib_name"] = args.l[0]
        gen_files = generate_library(norm_dir, fmt_dict)
    elif args.hdr is not None:  # test for .h file
        fmt_dict["header_name"] = args.hdr[0]
        gen_files = generate_header_file(norm_dir, fmt_dict)
    elif args.src is not None:  # test for .cpp file
        fmt_dict["source_name"] = args.src[0]
        gen_files = generate_source_file(norm_dir, fmt_dict)
    elif args.create_project: # test for project
        gen_files = generate_project(fmt_dict)
        # cmake is already handled so do not handle it elsewhere
        args.edit_cmake = False
    else:  # invalid state, how did we get here???
        print("Invalid State, should not be able to reach this point")
        sys.exit()
    # Add the generated files to the appropriate DirData.cmake files
    if args.edit_cmake:
        cmake_file_list = add_to_dir_data(gen_files)
        gen_files.extend(cmake_file_list)
    # tell the user what files we created / added to git
    if gen_files:
        if args.add_to_git:
            add_to_git(gen_files)
            print("\nAdded the following created or modified files to the git repository:")
        else:
            print("\n Created or modified the following files:")
        for file_path in gen_files:
            print("\t%s" % file_path)
        print("")


def get_rel_for_cmake(path):
    """Gets the path of x relative to the config base directory"""
    rel_dirs = [os.path.join(CONFIG["base_dir"], x)
                for x in CONFIG["base_subdirs"]]
    if len(rel_dirs) == 0:
        rel_dirs.append(CONFIG["base_dir"])
    shortest = os.path.relpath(path, rel_dirs[0])
    shortestLen = len(shortest)
    for rd in rel_dirs[1:]:
        temp = os.path.relpath(path, rd)
        if len(temp) < shortestLen:
            shortest = temp
            shortestLen = len(temp)
    return shortest


def ft_str(fn):
    """A simple function to extract the correct format string for some cmake
    file name string calculation"""
    if "$" in fn:
        return "\n     {0:s}".format(fn)
    else:
        return "\n     ./{0:s}".format(get_rel_for_cmake(fn))


def add_to_dir_data(paths):
    """creates or modifies DirData.cmake files as needed to include the newly
    created files"""
    # The files are in the same path, so grab the dir of the first one
    file_dir = os.path.dirname(paths[0])
    cmk_file = os.path.join(file_dir, "DirData.cmake")
    hdrs = []
    srcs = []
    for pth in paths:
        if pth.endswith(CONFIG["src_ext"]):
            srcs.append(pth)
        elif pth.endswith(CONFIG["head_ext"]):
            hdrs.append(pth)
    return recursively_handle_cmake(cmk_file, hdrs, srcs, [])


def recursively_handle_cmake(cmk_file, hdrs, srcs, includes):
    """handles adding files to DirData.cmake, creating a new one if needed
    and recursively moving down the directory tree to reach the first DirData
    that it does not have to make"""
    modded_files = []
    if os.path.exists(cmk_file):  # we can terminate the recursion here
        add_to_cmake_file(cmk_file, hdrs, srcs, includes)
        modded_files.append(cmk_file)
    else:
        new_hdrs, new_srcs = create_cmake_file(cmk_file, hdrs, srcs, includes)
        modded_files.append(cmk_file)
        new_dir = os.path.dirname(os.path.dirname(cmk_file))
        new_cmk_file = os.path.join(new_dir, "DirData.cmake")
        temp = recursively_handle_cmake(new_cmk_file, new_hdrs,
                                        new_srcs, [cmk_file])
        modded_files.extend(temp)
    return modded_files


def create_cmake_file(cmk_file, hdrs, srcs, includes):
    """Creates a new DirData.cmake file from scratch"""
    fmt_dict = {}
    fmt_dict["inc_lines"] = ""
    if includes:
        inc_line = "\ninclude(./{0:s})".format(get_rel_for_cmake(includes[0]))
        fmt_dict["inc_lines"] = inc_line
    fmt_dict["hdr_files"] = "".join([ft_str(fn) for fn in hdrs])
    fmt_dict["src_files"] = "".join([ft_str(fn) for fn in srcs])
    # cmk_rel_path = get_rel_for_cmake(os.path.dirname(cmk_file))
    cmk_rel_path = os.path.dirname(cmk_file)
    hdrs_name, srcs_name = make_list_names(cmk_rel_path)
    fmt_dict["srcs_name"] = srcs_name
    fmt_dict["hdrs_name"] = hdrs_name
    with open(cmk_file, 'w') as outfile:
        outfile.write(TEMPLATES["cmake"].format(**fmt_dict))
    return (["${{{0:s}}}".format(hdrs_name)], ["${{{0:s}}}".format(srcs_name)])


def add_to_cmake_file(cmk_file, hdrs, srcs, includes):
    """adds simple files to a DirData.cmake file without adding an include to
    the DirData.cmake"""
    cmake_lines = ["".join([ft_str(fn) for fn in hdrs]),
                   "".join([ft_str(fn) for fn in srcs])]
    cmake_str = ""
    with open(cmk_file, 'r') as infile:
        cmake_str = infile.read()
    # handle the case where we also need to add an include
    if includes:
        inc_ind = cmake_str.find("\n")
        inc_line = "\ninclude(./{0:s})".format(get_rel_for_cmake(includes[0]))
        temp = cmake_str[0:inc_ind] + inc_line + cmake_str[inc_ind:]
        cmake_str = temp
    # handle the normal stuff to add the passed files / variables
    names = make_list_names(get_rel_for_cmake(os.path.dirname(cmk_file)))
    brk_ind = cmake_str.find(names[0]) + len(names[0])
    temp = cmake_str[0:brk_ind] + cmake_lines[0] + cmake_str[brk_ind:]
    brk_ind = temp.find(names[1]) + len(names[1])
    cmake_str = temp[0:brk_ind] + cmake_lines[1] + temp[brk_ind:]
    with open(cmk_file, 'w') as outfile:
        outfile.write(cmake_str)


def make_list_names(rel_dir):
    """Generates a cmake list variable name for a given directory"""
    list_name = rel_dir
    # replace path characters with _
    list_name = list_name.replace("/", "_")
    list_name = list_name.replace(".", "_")
    # replace multiple _ with one _
    list_name = ''.join("_" if a == "_" else ''.join(b) for a, b in
                        groupby(list_name))
    lst_name_base = list_name.upper()
    if lst_name_base[0] == '_':
        lst_name_base = lst_name_base[1:]
    return (lst_name_base + "_HDRS", lst_name_base + "_SRCS")


def normalize_and_check_dir(given_dir):
    """normalize the directory path and make sure it exists, if it does not
    exist, then create it"""
    CONFIG["base_dir"] = os.path.abspath(CONFIG["base_dir"])
    given_dir_abs = os.path.abspath(given_dir)
    abs_dir = None
    if not given_dir_abs.startswith(CONFIG["base_dir"]):
        abs_dir = os.path.abspath(os.path.join(CONFIG["base_dir"], given_dir))
    else:
        abs_dir = given_dir_abs
    norm_dir = os.path.relpath(abs_dir)
    if not os.path.exists(norm_dir):
        try:
            os.makedirs(norm_dir)
        except OSError as exc:
            if exc.errno == errno.EEXIST and os.path.isdir(norm_dir):
                pass
            else:
                raise
    return norm_dir


def generate_template_class(direc, fmt_dict):
    """Generate a template class header file"""
    # here we make only one file, the header file
    # extract the class name and template parameters
    class_name = fmt_dict["class_name"]
    # make the name of the header file from the class name
    header_name = (class_name + CONFIG["head_ext"])
    # make the path for the header file
    header_path = os.path.join(direc, header_name)
    # get the template parameter list
    tmpl_params = fmt_dict["template_param_list"]
    fmtstr1 = "class {0:s}"
    fmtstr2 = "* @tparam {0:s} "
    if len(tmpl_params) > 1:
        fmt_dict["template_parameter_list"] = ", ".join([fmtstr1.format(x)
                                                         for x in tmpl_params])
        dox_list = [fmtstr2.format(x) for x in tmpl_params]
        fmt_dict["template_parameter_doxygen"] = "\n".join(dox_list)
    else:
        fmt_dict["template_parameter_list"] = fmtstr1.format(tmpl_params[0])
        fmt_dict["template_parameter_doxygen"] = fmtstr2.format(tmpl_params[0])
    # test to make sure we will not overwrite a header file
    test_for_writable(header_path, "template class header file")
    # get the name for the macro guard
    fmt_dict["macro_guard_name"] = make_macro_guard_var_name(header_path)
    # write the header file
    out_file = open(header_path, 'w')
    out_file.write(TEMPLATES["template_header"].format(**fmt_dict))
    out_file.close()
    # return the paths of the generated files
    return [header_path]


def generate_class(direc, fmt_dict):
    """generate a class header and source file"""
    # here we make two files, the header and cpp file
    # get the class name
    class_name = fmt_dict["class_name"]
    # get the name of the header file
    header_name = (class_name + CONFIG["head_ext"])
    fmt_dict["header_path"] = header_name
    # get the name of the class file
    source_name = (class_name + CONFIG["src_ext"])
    # get the path of the header file
    header_path = os.path.join(direc, header_name)
    # get the path of the source file
    source_path = os.path.join(direc, source_name)
    # test to make sure we will not overwrite a header file
    test_for_writable(header_path, "class header file")
    test_for_writable(source_path, "class source file")
    # get the name for the macro guard
    fmt_dict["macro_guard_name"] = make_macro_guard_var_name(header_path)
    # write the header file
    out_file = open(header_path, 'w')
    out_file.write(TEMPLATES["class_header"].format(**fmt_dict))
    out_file.close()
    # write the source file
    out_file = open(source_path, 'w')
    out_file.write(TEMPLATES["class_source"].format(**fmt_dict))
    out_file.close()
    # return the paths of the generated files
    return [header_path, source_path]


def generate_header_class(direc, fmt_dict):
    """generate a class header and source file"""
    # here we make two files, the header and cpp file
    # get the class name
    class_name = fmt_dict["class_name"]
    # get the name of the header file
    header_name = (class_name + CONFIG["head_ext"])
    fmt_dict["header_path"] = header_name
    # get the name of the class file
    source_name = (class_name + CONFIG["src_ext"])
    # get the path of the header file
    header_path = os.path.join(direc, header_name)
    # test to make sure we will not overwrite a header file
    test_for_writable(header_path, "header only class header file")
    # get the name for the macro guard
    fmt_dict["macro_guard_name"] = make_macro_guard_var_name(header_path)
    # write the header file
    out_file = open(header_path, 'w')
    out_file.write(TEMPLATES["class_header"].format(**fmt_dict))
    out_file.close()
    # return the paths of the generated files
    return [header_path]


def generate_abc(direc, fmt_dict):
    """generate an abstract base class header"""
    # here we make two files, the header and cpp file
    # get the class name
    class_name = fmt_dict["class_name"]
    # get the name of the header file
    header_name = (class_name + CONFIG["head_ext"])
    fmt_dict["header_path"] = header_name
    # get the path of the header file
    header_path = os.path.join(direc, header_name)
    # test to make sure we will not overwrite a header file
    test_for_writable(header_path, "abstract base class header file")
    # get the name for the macro guard
    fmt_dict["macro_guard_name"] = make_macro_guard_var_name(header_path)
    # write the header file
    out_file = open(header_path, 'w')
    out_file.write(TEMPLATES["class_header"].format(**fmt_dict))
    out_file.close()
    # return the paths of the generated files
    return [header_path]


def generate_factory(direc, fmt_dict):
    """generate a factory for sub classes of a common base class"""
    # here we make two files, the header and cpp file
    # get the class name
    factory_type = fmt_dict["factory_type"]
    # get the names of the files
    header_name = (factory_type + "Factory" + CONFIG["head_ext"])
    source_name = (factory_type + "Factory" + CONFIG["src_ext"])
    internal_header_name = (factory_type + "FactoryInternal" +
                            CONFIG["head_ext"])
    fmt_dict["header_path"] = header_name
    fmt_dict["internal_header_path"] = internal_header_name
    # get the path of the header file
    header_path = os.path.join(direc, header_name)
    source_path = os.path.join(direc, source_name)
    internal_header_path = os.path.join(direc, internal_header_name)
    # test to make sure we will not overwrite a file
    test_for_writable(header_path, "factory header file")
    test_for_writable(source_path, "factory source file")
    test_for_writable(internal_header_path, "factory internal header file")
    # get the name for the macro guard
    fmt_dict["macro_guard_name"] = make_macro_guard_var_name(header_path)
    # write the header file
    out_file = open(header_path, 'w')
    out_file.write(TEMPLATES["factory_header"].format(**fmt_dict))
    out_file.close()
    # get the name for the other macro guard
    fmt_dict["macro_guard_name"] = make_macro_guard_var_name(
        internal_header_path)
    # write the internal header file
    out_file = open(internal_header_path, 'w')
    out_file.write(TEMPLATES["factory_internal_header"].format(**fmt_dict))
    out_file.close()
    # write the source file
    out_file = open(source_path, 'w')
    out_file.write(TEMPLATES["factory_source"].format(**fmt_dict))
    out_file.close()
    # return the paths of the generated files
    return [header_path, source_path, internal_header_path]


def generate_registry(direc, fmt_dict):
    """generate a factory for sub classes of a common base class"""
    # here we make two files, the header and cpp file
    # get the class name
    registry_type = fmt_dict["registry_type"]
    # get the names of the files
    header_name = (registry_type + "Registry" + CONFIG["head_ext"])
    source_name = (registry_type + "Registry" + CONFIG["src_ext"])
    fmt_dict["header_path"] = header_name
    # get the path of the header file
    header_path = os.path.join(direc, header_name)
    source_path = os.path.join(direc, source_name)
    # test to make sure we will not overwrite a file
    test_for_writable(header_path, "registry header file")
    test_for_writable(source_path, "registry source file")
    # get the name for the macro guard
    fmt_dict["macro_guard_name"] = make_macro_guard_var_name(header_path)
    # write the header file
    out_file = open(header_path, 'w')
    out_file.write(TEMPLATES["registry_header"].format(**fmt_dict))
    out_file.close()
    # write the source file
    out_file = open(source_path, 'w')
    out_file.write(TEMPLATES["registry_source"].format(**fmt_dict))
    out_file.close()
    # return the paths of the generated files
    return [header_path, source_path]


def generate_library(direc, fmt_dict):
    """generate a library of functions header and source file"""
    # here we make two files, the header and cpp file
    # get the library name
    lib_name = fmt_dict["lib_name"]
    # get the name of the header file
    header_name = (lib_name + CONFIG["head_ext"])
    fmt_dict["header_path"] = header_name
    # get the name of the source file
    source_name = (lib_name + CONFIG["src_ext"])
    # get the path of the header file
    header_path = os.path.join(direc, header_name)
    # get the path of the source file
    source_path = os.path.join(direc, source_name)
    # test to make sure we will not overwrite a header file
    test_for_writable(header_path, "library header file")
    # test to make sure we will not overwrite a source file
    test_for_writable(source_path, "library source file")
    # get the name for the macro guard
    fmt_dict["macro_guard_name"] = make_macro_guard_var_name(header_path)
    # write the header file
    out_file = open(header_path, 'w')
    out_file.write(TEMPLATES["lib_header"].format(**fmt_dict))
    out_file.close()
    # write the source file
    out_file = open(source_path, 'w')
    out_file.write(TEMPLATES["lib_source"].format(**fmt_dict))
    out_file.close()
    # return the paths of the generated files
    return [header_path, source_path]


def generate_header_file(direc, fmt_dict):
    """generate a header file"""
    # get the header name
    header_name = fmt_dict["header_name"]
    # check if the user added the .h to the end of the header name
    if not header_name.endswith(CONFIG["head_ext"]):
        # make the name of the header file from the class name
        header_name = (header_name + CONFIG["head_ext"])
    # make the path for the header file
    header_path = os.path.join(direc, header_name)
    # test to make sure we will not overwrite a header file
    test_for_writable(header_path, "bare header file")
    # get the name for the macro guard
    fmt_dict["macro_guard_name"] = make_macro_guard_var_name(header_path)
    # write the header file
    out_file = open(header_path, 'w')
    out_file.write(TEMPLATES["plain_header"].format(**fmt_dict))
    out_file.close()
    # return the paths of the generated files
    return [header_path]


def generate_source_file(direc, fmt_dict):
    """generate a source file"""
    # get the header name
    source_name = fmt_dict["source_name"]
    # check if the user added the .h to the end of the header name
    if not source_name.endswith(CONFIG["src_ext"]):
        # make the name of the header file from the class name
        source_name = (source_name + CONFIG["src_ext"])
    # make the path for the header file
    source_path = os.path.join(direc, source_name)
    # test to make sure we will not overwrite a source file
    test_for_writable(source_path, "bare source file")
    # write the source file
    out_file = open(source_path, 'w')
    out_file.write(TEMPLATES["plain_source"].format(**fmt_dict))
    out_file.close()
    # return the paths of the generated files
    return [source_path]


def generate_singleton(direc, fmt_dict):
    """generate a singleton header and source file"""
    # here we make two files, the header and cpp file
    # get the class name
    class_name = fmt_dict["class_name"]
    # get the name of the header file
    header_name = (class_name + CONFIG["head_ext"])
    fmt_dict["header_path"] = header_name
    # get the name of the implementation file
    source_name = (class_name + CONFIG["src_ext"])
    # get the path of the header file
    header_path = os.path.join(direc, header_name)
    # get the path of the source file
    source_path = os.path.join(direc, source_name)
    # test to make sure we will not overwrite a header file
    test_for_writable(header_path, "singleton header file")
    test_for_writable(source_path, "singleton source file")
    # get the name for the macro guard
    fmt_dict["macro_guard_name"] = make_macro_guard_var_name(header_path)
    # write the header file
    out_file = open(header_path, 'w')
    out_file.write(TEMPLATES["singleton_header"].format(**fmt_dict))
    out_file.close()
    # write the source file
    out_file = open(source_path, 'w')
    out_file.write(TEMPLATES["singleton_source"].format(**fmt_dict))
    out_file.close()
    # return the paths of the generated files
    return [header_path, source_path]


def generate_project(fmt_dict):
    """Generate a project"""
    generated_files = []
    # first generate the appropriate sub-directories
    base_project_dir = os.path.join("./", CONFIG["base_dir"])
    cmake_dir = os.path.join(base_project_dir, "cmake")
    src_dir = os.path.join(base_project_dir, "src")
    tooling_dir = os.path.join(base_project_dir, "tooling")
    cfg_dir = os.path.join(src_dir, "BuildConfig")
    if not os.path.exists(base_project_dir):
        os.makedirs(base_project_dir)
    if not os.path.exists(cmake_dir):
        os.makedirs(cmake_dir)
    if not os.path.exists(src_dir):
        os.makedirs(src_dir)
    if not os.path.exists(tooling_dir):
        os.makedirs(tooling_dir)
    if not os.path.exists(cfg_dir):
        os.makedirs(cfg_dir)
    #generate the paths to the files we will make
    main_path = os.path.join(src_dir, "main" + CONFIG["src_ext"])
    src_dir_data_path = os.path.join(src_dir, "DirData.cmake")
    cmakelists_path = os.path.join(base_project_dir, "CMakeLists.txt")
    base_dir_data_path = os.path.join(base_project_dir, "DirData.cmake")
    analyzer_path = os.path.join(cmake_dir, "Analyzers.cmake")
    tool_path = os.path.join(tooling_dir, "run-clang-tidy.py")
    build_opt_path = os.path.join(cmake_dir, "BldOpt.cmake")
    fmt_dict["build_config_header_input_name"] =\
        ("{project_name:s}BuildConfig".format(**fmt_dict) + CONFIG["head_ext"]
            + ".in")
    fmt_dict["build_config_header_name"] =\
        "{project_name:s}BuildConfig".format(**fmt_dict) + CONFIG["head_ext"]
    build_cfg_path = os.path.join(cfg_dir,
                                  fmt_dict["build_config_header_input_name"])

    # test that we can write all the paths we generated
    test_for_writable(main_path, "main source file")
    test_for_writable(src_dir_data_path, "source directory DirData.cmake")
    test_for_writable(cmakelists_path, "CMakeLists file")
    test_for_writable(base_dir_data_path, "base directory DirData.cmake")
    test_for_writable(tool_path, "run-clang-tidy script")
    test_for_writable(analyzer_path, "analyzer options path")
    test_for_writable(build_opt_path, "build options path")
    test_for_writable("./makefile", "convenience makefile")
    test_for_writable(build_cfg_path, "build configuration header")

    # now make the build configuration file
    fmt_dict['macro_guard_name'] = make_macro_guard_var_name(build_cfg_path)
    with open(build_cfg_path, 'w') as out_file:
        out_file.write(TEMPLATES["build_config_header_input"].format(**fmt_dict))
    generated_files.append(build_cfg_path)
    # now make the main source file
    with open(main_path, 'w') as out_file:
        out_file.write(TEMPLATES["main_source"].format(**fmt_dict))
    generated_files.append(main_path)
    # now make the main source directory
    with open(src_dir_data_path, 'w') as out_file:
        out_file.write(TEMPLATES["src_dir_dirdata"])
    generated_files.append(src_dir_data_path)
    # now make the project directory CMakeLists
    with open(cmakelists_path, 'w') as out_file:
        out_file.write(TEMPLATES["cmakelists_source"].format(**fmt_dict))
    generated_files.append(cmakelists_path)
    # now make the project directory DirData.cmake
    with open(base_dir_data_path, 'w') as out_file:
        out_file.write(TEMPLATES["base_dir_dirdata"].format(**fmt_dict))
    generated_files.append(base_dir_data_path)
    # now make the clang-tidy config file
    with open(tool_path, 'w') as out_file:
        out_file.write(TEMPLATES["clang_tidy_python_script"])
    generated_files.append(tool_path)
    # now make the analyzer cmake file
    with open(analyzer_path, 'w') as out_file:
        out_file.write(TEMPLATES["cmake_analyzers_with_checks"])
    generated_files.append(analyzer_path)
    # now make the build options cmake file
    with open(build_opt_path, 'w') as out_file:
        out_file.write(TEMPLATES["cmake_build_options"])
    generated_files.append(build_opt_path)
    # now make the convenience makefile
    with open("./makefile", 'w') as out_file:
        out_file.write(TEMPLATES["bottom_level_makefile"].format(**fmt_dict))
    generated_files.append("./makefile")
    # now return what we made
    return generated_files


def calculate_namespace_strings(ns_parse_str):
    """Generate a list of namespaces from the scope resolution operator sep
    list that is passed in"""
    # namespaces = ns_parse_str.split("::")
    # open_str = ""
    # close_lst = []
    # for ns in namespaces:
    #     open_str += "\nnamespace {0:s}\n{{\n".format(ns)
    #     close_lst.insert(0, "\n}} // namespace {0:s}\n".format(ns))
    # return open_str, "".join(close_lst)
    open_str = f"\nnamespace {ns_parse_str:s}\n{{\n"
    close_str = f"\n}} // namespace {ns_parse_str:s}\n"
    return open_str, close_str


def make_macro_guard_var_name(header_path):
    """makes the appropriate macro guard name from the path and project name"""
    guard_name = ""
    # handle the possible path starting cases
    if header_path.upper().startswith("./" + CONFIG["project_name"].upper()):
        guard_name = header_path[2:]
    elif header_path.upper().startswith("./"):
        guard_name = (CONFIG["project_name"] + "/" + header_path[2:])
    elif header_path.upper().startswith("/" + CONFIG["project_name"].upper()):
        guard_name = header_path[1:]
    elif header_path.upper().startswith("/"):
        guard_name = (CONFIG["project_name"] + "/" + header_path[1:])
    elif header_path.upper().startswith(CONFIG["project_name"].upper()):
        guard_name = header_path
    else:
        guard_name = (CONFIG["project_name"] + "/" + header_path)
    # replace path characters with _
    guard_name = guard_name.replace("/", "_")
    guard_name = guard_name.replace(".", "_")
    guard_name = guard_name.replace("-", "_")
    # replace multiple _ with one _
    guard_name =\
        ''.join("_" if a == "_" else ''.join(b) for a, b in
                groupby(guard_name))
    return guard_name.upper()


def test_for_writable(path, file_desc):
    """Test if the file can be written, exit if not"""
    if os.path.isfile(path) and not CONFIG["Allow Overwrite"]:
        print("\n\nCannot create", file_desc, ", another file with the")
        print("same path already exists, exitting\n\n")
        sys.exit()


def add_to_git(file_list):
    """Adds a set of files to the git via the git add command"""
    for file_path in file_list:
        cmd_str = ("git add %s" % file_path)
        proc = subprocess.Popen(cmd_str, shell=True, stdout=subprocess.PIPE)
        proc.wait()
    return


# help strings for command line options and the like
PARSER_DESCRIPTION =\
    "Generate placeholder c++ files with the appropriate macro guards, "\
    "templates, and other accoutrement"
DIR_HELP =\
    "Directory to place file(s) in, given relative to config option"\
    " CONFIG[\"base_dir\"], if not passed, will default to '.' (relative to"\
    " the base directory) This option should be left blank for project creation"
TC_HELP =\
    "Create a template class .h file with the class ClassName already "\
    "prepared within it. The template class will have the parameters given"\
    " in TemplateParameters"
C_HELP =\
    "Create a class .h and .cpp file with the class ClassName prepared within"\
    " it"
HC_HELP =\
    "Create a class .h file with the class ClassName prepared within it"
A_HELP =\
    "Create a class .h file with the class ClassName Abstract base class "\
    "prepared within it"
S_HELP =\
    "Create a class .h and .cpp file with the class ClassName prepared as a"\
    " singleton within them"
FACTORY_HELP =\
    "Create a factory for sub classes derived from a common base class which"\
    " allows the construction of the appropriate subclass using just the"\
    " name string. The factory will be named <FactoryType>Factory"
REG_HELP =\
    "Create a registry that classes derived from a particular base class can"\
    " register with under a name unique to that instance. In that way,"\
    " already constructed objects can have their pointers retrieved by other"\
    " objects merely using their name. The registry will be named:"\
    " <RegistryType>Registry"
PROJ_HELP =\
    "Create a new, empty, project with a main.cpp cmake_lists, etc"
NS_HELP =\
    "Set the namespace(s) of any source generated. Nested namespaces are given"\
    " as follows: Outer::Nested::etc"
L_HELP =\
    "Create a .h and .cpp file to store a library of functions within the"\
    "given directory named LibraryName.(h|cpp)"
HF_HELP = "Create a .h file with name HeaderName.h"
CF_HELP = "Create a .cpp file with name SourceName.cpp"
SKIP_CMAKE_HELP = "Skip Updating CMake Files"
SKIP_GIT_HELP = "Skip Adding Created / Modified files to git"
OVERWRITE_HELP = "Overwrite already existing files"

if __name__ == "__main__":
    main()
