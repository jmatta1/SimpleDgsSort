"""File for templates"""
# various templates for the file creation system, modifying these would require
# substantial changes to code in ct.py
TEMPLATES = {}
# Template for template class header files
TEMPLATES["template_header"] =\
    """/***************************************************************************//**
********************************************************************************
**
** @author {author_name:s}
** @date {day:s} {month_abbrev:s}, {year:s}
**
** @copyright Copyright (C) {year:s} {copyright_holder:s}
**
********************************************************************************
*******************************************************************************/
#ifndef {macro_guard_name:s}
#define {macro_guard_name:s}

// includes for C system headers
// includes for C++ system headers
// includes from other libraries
// includes from {project_name:s}
{namespace_open:s}
/*!
* @brief
{template_parameter_doxygen:s}
*/
template <{template_parameter_list:s}>
class {class_name:s}
{{
public:

private:

}};
{namespace_close:s}
#endif  // {macro_guard_name:s}
"""

# Template for class header files
TEMPLATES["class_header"] =\
    """/***************************************************************************//**
********************************************************************************
**
** @author {author_name:s}
** @date {day:s} {month_abbrev:s}, {year:s}
**
** @copyright Copyright (C) {year:s} {copyright_holder:s}
**
********************************************************************************
*******************************************************************************/
#ifndef {macro_guard_name:s}
#define {macro_guard_name:s}

// includes for C system headers
// includes for C++ system headers
// includes from other libraries
// includes from {project_name:s}
{namespace_open:s}
/*!
* @brief
*/
class {class_name:s}
{{
public:

private:

}};
{namespace_close:s}
#endif  // {macro_guard_name:s}
"""

# Template for class cpp files
TEMPLATES["class_source"] =\
    """/***************************************************************************//**
********************************************************************************
**
** @author {author_name:s}
** @date {day:s} {month_abbrev:s}, {year:s}
**
** @copyright Copyright (C) {year:s} {copyright_holder:s}
**
********************************************************************************
*******************************************************************************/
#include"{header_path:s}"
// includes for C system headers
// includes for C++ system headers
// includes from other libraries
// includes from {project_name:s}
{namespace_open:s}
{namespace_close:s}
"""

# Template for library header files
TEMPLATES["lib_header"] =\
    """/***************************************************************************//**
********************************************************************************
**
** @author {author_name:s}
** @date {day:s} {month_abbrev:s}, {year:s}
**
** @copyright Copyright (C) {year:s} {copyright_holder:s}
**
********************************************************************************
*******************************************************************************/
#ifndef {macro_guard_name:s}
#define {macro_guard_name:s}

// includes for C system headers
// includes for C++ system headers
// includes from other libraries
// includes from {project_name:s}
{namespace_open:s}
{namespace_close:s}
#endif  // {macro_guard_name:s}
"""

# Template for library class files
TEMPLATES["lib_source"] =\
    """/***************************************************************************//**
********************************************************************************
**
** @author {author_name:s}
** @date {day:s} {month_abbrev:s}, {year:s}
**
** @copyright Copyright (C) {year:s} {copyright_holder:s}
**
********************************************************************************
*******************************************************************************/
#include"{header_path:s}"
// includes for C system headers
// includes for C++ system headers
// includes from other libraries
// includes from {project_name:s}
{namespace_open:s}
{namespace_close:s}
"""

# Template for plain header files
TEMPLATES["plain_header"] =\
    """/***************************************************************************//**
********************************************************************************
**
** @author {author_name:s}
** @date {day:s} {month_abbrev:s}, {year:s}
**
** @copyright Copyright (C) {year:s} {copyright_holder:s}
**
********************************************************************************
*******************************************************************************/
#ifndef {macro_guard_name:s}
#define {macro_guard_name:s}

// includes for C system headers
// includes for C++ system headers
// includes from other libraries
// includes from {project_name:s}
{namespace_open:s}
{namespace_close:s}
#endif  // {macro_guard_name:s}
"""

# Template for plain cpp files
TEMPLATES["plain_source"] =\
    """/***************************************************************************//**
********************************************************************************
**
** @author {author_name:s}
** @date {day:s} {month_abbrev:s}, {year:s}
**
** @copyright Copyright (C) {year:s} {copyright_holder:s}
**
********************************************************************************
*******************************************************************************/

// includes for C system headers
// includes for C++ system headers
// includes from other libraries
// includes from {project_name:s}
{namespace_open:s}
{namespace_close:s}
"""

# Template for DirData.cmake files
TEMPLATES["cmake"] =\
    """# pull the subdirectory information if it is needed{inc_lines:s}

# Source files from this directory
list(APPEND {srcs_name:s} {src_files:s})

# Header files from this directory
list(APPEND {hdrs_name:s} {hdr_files:s})
"""

# template for factory headers
TEMPLATES["factory_header"] =\
    """/***************************************************************************//**
********************************************************************************
**
** @author {author_name:s}
** @date {day:s} {month_abbrev:s}, {year:s}
**
** @copyright Copyright (C) {year:s} {copyright_holder:s}
**
********************************************************************************
*******************************************************************************/
#ifndef {macro_guard_name:s}
#define {macro_guard_name:s}

// includes for C system headers
// includes for C++ system headers
#include<string> //for string
#include<map> //for map
// includes from other libraries
// includes from {project_name:s}
#include"{internal_header_path}"
{namespace_open:s}
//forward declare the base class
class {base_class:s};

/*!
 * @brief A convenience function to make the appropriate {factory_type:s} class from the passed name
 * @param name The name of the object type to be created
 * @return A pointer to the newly constructed object
 */
{base_class:s}* construct{factory_type:s}(const std::string& name);

/*!
 * @brief A class that is used to register and construct {base_class:s}
 * derived objects that are needed
 */
class {factory_type:s}Factory
{{
public:
    /*!
     * @brief Deleted copy constructor to prevent breaks in the singleton behaviour
     */
    {factory_type:s}Factory(const {factory_type:s}Factory&) = delete;

    /*!
     * @brief Deleted move constructor to prevent breaks in the singleton behaviour
     */
    {factory_type:s}Factory(const {factory_type:s}Factory&&) = delete;

    /*!
     * @brief Delete copy assignment to prevent breaks in the singleton behaviour
     */
    {factory_type:s}Factory& operator=(const {factory_type:s}Factory &)  = delete;

    /*!
     * @brief Delete move assignment to prevent breaks in the singleton behaviour
     */
    {factory_type:s}Factory& operator=(const {factory_type:s}Factory &&)  = delete;

    /*!
     * @brief Default destuctor
     */
    ~{factory_type:s}Factory() = default;

    /*!
     * @brief Returns the instance of the singleton, which is created on the first call
     * @return The singleton instance
     */
    static {factory_type:s}Factory& getInstance();

    /*!
     * @brief Used to register a new builder with the factory by the recorder
     * @param name the lookup name of the class
     * @param bb a pointer to the builder that will make the class as needed
     */
    void register{factory_type:s}(const std::string& name, {factory_type:s}FactoryInternal::{factory_type:s}BuilderBase* bb);

    /*!
     * @brief makes a new instance of the named {factory_type:s}
     * @param name The name to lookup the {factory_type:s} by
     * @return The newly created {factory_type:s} object, if the name was found, nullptr otherwise
     */
    {base_class:s}* make{factory_type:s}(const std::string& name);


private:
    /*!
     * @brief Private constructor to prevent anything other than the class itself from making an instance
     */
    {factory_type:s}Factory() = default;

    std::map<std::string, {factory_type:s}FactoryInternal::{factory_type:s}BuilderBase* > builderList; ///<List of names and their builders
}};

/*!
 * @brief A struct that is used to register a class with the Factory
 * @tparam {factory_type}Type the type to be registered by this classes instatiation
 *
 * To register your {base_class:s} derived class with the Factory as the final step
 * to allowing it to be looked up and created at need via string, use the following line
 * at the top of your source file
 *
 * static {namespace_name:s}::{factory_type:s}FactoryRegistrar<YourClassName> YourClassRegistration("NameForClass");
 */
template <class {factory_type}Type>
struct {factory_type:s}FactoryRegistrar
{{
    /*!
     * @brief Constructor for the registrar which registers the {factory_type}Type class in the Factory
     * @param name The name that the {factory_type}Type class is registered under, full namespace listing and the classname
     * is a recommended form so that all names are unique and there are no collisions
     */
    explicit {factory_type:s}FactoryRegistrar(const std::string& name)
    {{
            {factory_type:s}Factory::getInstance().register{factory_type:s}(name,
                new {factory_type:s}FactoryInternal::{factory_type:s}Builder<{factory_type}Type>());
    }}
}};
{namespace_close:s}
#endif  // {macro_guard_name:s}
"""

# template for factory source files
TEMPLATES["factory_source"] =\
    """/***************************************************************************//**
********************************************************************************
**
** @author {author_name:s}
** @date {day:s} {month_abbrev:s}, {year:s}
**
** @copyright Copyright (C) {year:s} {copyright_holder:s}
**
********************************************************************************
*******************************************************************************/
#include"{header_path:s}"
// includes for C system headers
// includes for C++ system headers
#include<sstream>
// includes from other libraries
// includes from {project_name:s}
{namespace_open:s}
{base_class:s}* construct{factory_type:s}(const std::string& name)
{{
    return ({factory_type:s}Factory::getInstance().make{factory_type:s}(name));
}}

{factory_type:s}Factory& {factory_type:s}Factory::getInstance()
{{
    static {factory_type:s}Factory instance;
    return instance;
}}

void {factory_type:s}Factory::register{factory_type:s}(const std::string& name,
                                           {factory_type:s}FactoryInternal::{factory_type:s}BuilderBase* bb)
{{
    auto iter = builderList.find(name);
    if(iter != builderList.end())
    {{
        //Throw an exception if there is an attempt to register two builders under the same name
        //since static initialization in the recorder occurs at program start before entry into
        //int main(argc, argv) there can be no try catch statements, so this amounts to
        //deliberately crashing the program during startup in the case of a conflicting module name
        std::ostringstream errorMaker;
        errorMaker << "\\nInvalid attempt to register a second {factory_type:s} with the name: \\""<<name<<"\\"!";
        throw std::invalid_argument(errorMaker.str());
    }}
    builderList[name] = bb;
}}

{base_class:s}* {factory_type:s}Factory::make{factory_type:s}(const std::string& name)
{{
    auto iter = builderList.find(name);
    if(iter != builderList.end())
    {{//if we could find the desired type builder set the retVal to a created instance
        return iter->second->make{factory_type:s}();
    }}
    return nullptr;
}}
{namespace_close:s}
"""

# Template for the factory's internal header file
TEMPLATES["factory_internal_header"] =\
    """/***************************************************************************//**
********************************************************************************
**
** @author {author_name:s}
** @date {day:s} {month_abbrev:s}, {year:s}
**
** @copyright Copyright (C) {year:s} {copyright_holder:s}
**
********************************************************************************
*******************************************************************************/
#ifndef {macro_guard_name:s}
#define {macro_guard_name:s}

// includes for C system headers
// includes for C++ system headers
// includes from other libraries
// includes from {project_name:s}
{namespace_open:s}
class {base_class:s};

namespace {factory_type:s}FactoryInternal
{{

struct {factory_type}BuilderBase
{{
    /*!
     * @brief Abstract base class does need much of a destructor
     */
    virtual ~{factory_type}BuilderBase() = default;

    /*!
     * @brief pure virtual function to make {factory_type:s} on demand
     * @return a pointer to the created object base class
     */
    virtual {base_class:s}* make{factory_type}() const = 0;
}};

template <class {factory_type}Type>
struct {factory_type}Builder : public {factory_type}BuilderBase
{{
    /*!
     * @brief Class does need much of a destructor
     */
    ~{factory_type}Builder() final = default;

    /*!
     * @brief Concrete implementation to generate correct {factory_type:s} at the right time
     * @return A pointer to the created {factory_type:s} object
     */
    {base_class:s}* make{factory_type}() const final {{return new {factory_type}Type();}}
}};

}}// namespace {factory_type:s}FactoryInternal
{namespace_close:s}
#endif  // {macro_guard_name:s}
"""

# template for registry header
TEMPLATES["registry_header"] =\
    """/***************************************************************************//**
********************************************************************************
**
** @author {author_name:s}
** @date {day:s} {month_abbrev:s}, {year:s}
**
** @copyright Copyright (C) {year:s} {copyright_holder:s}
**
********************************************************************************
*******************************************************************************/
#ifndef {macro_guard_name:s}
#define {macro_guard_name:s}

// includes for C system headers
// includes for C++ system headers
#include<string> //for string
#include<map> //for map
// includes from other libraries
// includes from {project_name:s}
{namespace_open:s}
//forward declare the base class
class {base_class:s};

/*!
 * @brief A convenience function to retrieve a registered object by its name
 * @param name The name of the object to be retrieved
 * @return A pointer to the retrieved object
 */
{base_class:s}* retrieve{registry_type:s}(const std::string& name);

/*!
 * @brief A convenience function to register an object under a given name
 * @param name The name of the object to be registered
 */
void register{registry_type:s}({base_class:s}* object, const std::string& name);

/*!
 * @brief A convenience function to register an object under a given name
 * @param name The name of the object to be unregistered
 */
void unregister{registry_type:s}(const std::string& name);

/*!
 * @brief A class that is used to register and retrieve {base_class:s}
 * derived objects as needed
 */
class {registry_type:s}Registry
{{
public:
    /*!
     * @brief Deleted copy constructor to prevent breaks in the singleton behaviour
     */
    {registry_type:s}Registry(const {registry_type:s}Registry&) = delete;

    /*!
     * @brief Deleted move constructor to prevent breaks in the singleton behaviour
     */
    {registry_type:s}Registry(const {registry_type:s}Registry&&) = delete;

    /*!
     * @brief Delete copy assignment to prevent breaks in the singleton behaviour
     */
    {registry_type:s}Registry& operator=(const {registry_type:s}Registry &)  = delete;

    /*!
     * @brief Delete move assignment to prevent breaks in the singleton behaviour
     */
    {registry_type:s}Registry& operator=(const {registry_type:s}Registry &&)  = delete;

    /*!
     * @brief Default destuctor
     */
    ~{registry_type:s}Registry() = default;

    /*!
     * @brief Returns the instance of the singleton, which is created on the first call
     * @return The singleton instance
     */
    static {registry_type:s}Registry& getInstance();

    /*!
     * @brief Used to register a new object with the registry, if an object is already present, it is overwritten
     * @param name the lookup name of the object
     * @param objPtr a base class pointer of the object to be retrieved
     */
    void register{registry_type:s}(const std::string& name, {base_class:s}* objPtr);

    /*!
     * @brief Used to remove a the named object from the registry
     * @param name the lookup name of the object
     */
    void unregister{registry_type:s}(const std::string& name);

    /*!
     * @brief finds the object registered to the name and retrieves it and returns a pointer to {registry_type:s}
     * @param name The name to lookup the {registry_type:s} by
     * @return A pointer to the registered {registry_type:s} object, if the name was found, nullptr otherwise
     */
    {base_class:s}* find{registry_type:s}(const std::string& name);


private:
    /*!
     * @brief Private constructor to prevent anything other than the class itself from making an instance
     */
    {registry_type:s}Registry() = default;

    std::map<std::string, {base_class:s}* > objectList; ///<List of names and their pointers
}};
{namespace_close:s}
#endif  // {macro_guard_name:s}
"""

# template for registry source file
TEMPLATES["registry_source"] =\
    """/***************************************************************************//**
********************************************************************************
**
** @author {author_name:s}
** @date {day:s} {month_abbrev:s}, {year:s}
**
** @copyright Copyright (C) {year:s} {copyright_holder:s}
**
********************************************************************************
*******************************************************************************/
#include"{header_path:s}"
// includes for C system headers
// includes for C++ system headers
// includes from other libraries
// includes from {project_name:s}
{namespace_open:s}
{base_class:s}* retrieve{registry_type:s}(const std::string& name)
{{
    return ({registry_type:s}Registry::getInstance().find{registry_type:s}(name));
}}

void register{registry_type:s}({base_class:s}* object, const std::string& name)
{{
    {registry_type:s}Registry::getInstance().register{registry_type:s}(name, object);
}}

void unregister{registry_type:s}(const std::string& name)
{{
    {registry_type:s}Registry::getInstance().unregister{registry_type:s}(name);
}}

{registry_type:s}Registry& {registry_type:s}Registry::getInstance()
{{
    static {registry_type:s}Registry instance;
    return instance;
}}

void {registry_type:s}Registry::register{registry_type:s}(const std::string& name, {base_class:s}* objPtr)
{{
    objectList[name] = objPtr;
}}

void {registry_type:s}Registry::unregister{registry_type:s}(const std::string& name)
{
    auto iter = objectList.find(name);
    if(iter != objectList.end())
    {
        objectList.erase(iter);
    }
}

{base_class:s}* {registry_type:s}Registry::find{registry_type:s}(const std::string& name)
{{
    auto iter = objectList.find(name);
    if(iter != objectList.end())
    {{
        return iter->second;
    }}
    return nullptr;
}}
{namespace_close:s}
"""

# template for main source file
TEMPLATES["main_source"] =\
    """/***************************************************************************//**
********************************************************************************
**
** @author {author_name:s}
** @date {day:s} {month_abbrev:s}, {year:s}
**
** @copyright Copyright (C) {year:s} {copyright_holder:s}
**
********************************************************************************
*******************************************************************************/
// includes for C system headers
// includes for C++ system headers
#include<iostream>
// includes from other libraries
// includes from {project_name:s}
#include"{build_config_header_name:s}"

int main(int argc, char* argv[])
{{
    std::cout << "\\n{project_name:s}\\n{project_title:s}"
              << "\\n     Version: " << {project_name_cap:s}_VERSION
              << "\\n  Build Type: " << {project_name_cap:s}_BUILD
              << "\\n  Git Commit: " << {project_name_cap:s}_COMMIT_SHA << std::endl;


    return 0;
}}
"""

# Template for class header files
TEMPLATES["singleton_header"] =\
    """/***************************************************************************//**
********************************************************************************
**
** @author {author_name:s}
** @date {day:s} {month_abbrev:s}, {year:s}
**
** @copyright Copyright (C) {year:s} {copyright_holder:s}
**
********************************************************************************
*******************************************************************************/
#ifndef {macro_guard_name:s}
#define {macro_guard_name:s}

// includes for C system headers
// includes for C++ system headers
// includes from other libraries
// includes from {project_name:s}
{namespace_open:s}
/*!
* @brief
*/
class {class_name:s}
{{
public:
    ~{class_name:s}();

    /*!
     * @brief Deleted copy constructor to prevent breaks in the singleton behaviour
     */
    {class_name:s}(const {class_name:s}&) = delete;

    /*!
     * @brief Deleted move constructor to prevent breaks in the singleton behaviour
     */
    {class_name:s}(const {class_name:s}&&) = delete;

    /*!
     * @brief Delete copy assignment to prevent breaks in the singleton behaviour
     */
    {class_name:s}& operator=(const {class_name:s}&)  = delete;

    /*!
     * @brief Delete move assignment to prevent breaks in the singleton behaviour
     */
    {class_name:s}& operator=(const {class_name:s}&&)  = delete;

    /*!
     * @brief Function to get the singleton instance, forcing construction/initialization
     * @return Reference to the instance
     */
    static {class_name:s}& getInstance();

private:
    /*!
     * @brief private constructor for singleton behavior
     */
    {class_name:s}();

}};
{namespace_close:s}
#endif  // {macro_guard_name:s}
"""

# Template for class cpp files
TEMPLATES["singleton_source"] =\
    """/***************************************************************************//**
********************************************************************************
**
** @author {author_name:s}
** @date {day:s} {month_abbrev:s}, {year:s}
**
** @copyright Copyright (C) {year:s} {copyright_holder:s}
**
********************************************************************************
*******************************************************************************/
#include"{header_path:s}"
// includes for C system headers
// includes for C++ system headers
// includes from other libraries
// includes from {project_name:s}
{namespace_open:s}

{class_name:s}::{class_name:s}()
{{
}}

{class_name:s}::~{class_name:s}()
{{
}}

{class_name:s}& {class_name:s}::getInstance()
{{
    static {class_name:s} instance;
    return instance;
}}

{namespace_close:s}
"""

# template for main CMakeLists file
TEMPLATES["cmakelists_source"] =\
    """cmake_minimum_required(VERSION 3.1)

###########################################################
##  Setup Project
###########################################################
project({project_name:s})

execute_process(COMMAND "git" "rev-parse" "-q" "HEAD" WORKING_DIRECTORY ${{CMAKE_CURRENT_SOURCE_DIR}} OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE {project_name_cap:s}_VERSION_SHA)

# The version number.
set({project_name_cap:s}_VERSION_MAJOR 0)
set({project_name_cap:s}_VERSION_MINOR 0)
set({project_name_cap:s}_VERSION_PATCH 0)
set({project_name_cap:s}_VERSION ${{{project_name_cap:s}_VERSION_MAJOR}}.${{{project_name_cap:s}_VERSION_MINOR}}.${{{project_name_cap:s}_VERSION_PATCH}})

message(STATUS "Version is: " ${{{project_name_cap:s}_VERSION}})

message(STATUS "Git revision is: " ${{{project_name_cap:s}_VERSION_SHA}})
message(STATUS "Src dir is: " ${{CMAKE_CURRENT_SOURCE_DIR}})

# Set the version number and build type in the header /Utility/OrchidConfig.h
configure_file("./src/BuildConfig/{build_config_header_input_name:s}"
               "${{PROJECT_BINARY_DIR}}/src/BuildConfig/{build_config_header_name:s}"
               @ONLY)

###########################################################
##  Setup C++ options
###########################################################
# set for c++ 14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # whether we want the gnu / clang extensions

###########################################################
##  Setup External Library Dependencies
###########################################################
# Add sub-directory for containing find module scripts that are not already provided
# for instance, the libUSB people do not bother providing a FindLibUSB file, claiming
# that cmake is a solution looking for a problem, ironically they provide xcode project
# files, visual studio project files, and auto-tools bindings... oh well, screw them
# I can put other files in here as well, like findTBB
list(APPEND CMAKE_MODULE_PATH "${{CMAKE_SOURCE_DIR}}/cmake/")


###########################################################
##  Setup Build Options
###########################################################
include(./cmake/BldOpt.cmake)

###########################################################
##  Setup Source Analysis Helpers Like Clang-Tidy
###########################################################
include(./cmake/Analyzers.cmake)

###########################################################
##  Setup include directories
###########################################################
# Add this root directory as an include directory so that we can reference
# our include files relative to it (less ../../../ business)
include_directories(./src)
# Add the generated file header directory in this way to allow files in the
# utility directory to access the generated file as if it was in their directory
include_directories("${{PROJECT_BINARY_DIR}}/src/BuildConfig")

###########################################################
##  Grab all the sub-directory file data
###########################################################
include(./DirData.cmake)
list(APPEND SRCS ${{{project_name_cap:s}_SRCS}})
list(APPEND HDRS ${{{project_name_cap:s}_HDRS}})

###########################################################
##  Override Source Groups
###########################################################
# So things look nice in QtCreator
SOURCE_GROUP("" FILES ${{SRCS}} ${{HDRS}})

###########################################################
##  Target Setup
###########################################################
# Add the executable and link it up
add_executable({exec_name:s} ${{SRCS}} ${{HDRS}})

if(CLANG_TIDY_EXE)
  set_target_properties({exec_name:s} PROPERTIES CXX_CLANG_TIDY "${{DO_CLANG_TIDY}}")
endif()

###########################################################
##  Install setup
###########################################################
get_filename_component(ABS_BIN_INSTALL_DIR "./.." ABSOLUTE)
install(TARGETS {exec_name:s} RUNTIME DESTINATION ${{ABS_BIN_INSTALL_DIR}})

"""

TEMPLATES["cmake_analyzers_with_checks"] =\
"""###########################################################
##  Setup clang tidy for lots of warnings and src analysis
###########################################################
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

find_program(
  CLANG_TIDY_EXE NAMES clang-tidy clang-tidy-12 clang-tidy-13 clang-tidy-14
  DOC "Path to clang-tidy executable")

find_program(
  RUN_CLANG_TIDY_PYTHON "${CMAKE_CURRENT_SOURCE_DIR}/tooling/run-clang-tidy.py"
  DOC "Path to python script to run clang-tidy on build database")

list(APPEND CLANG_TIDY_CHECKS_LIST
     "abseil-*,"
     "-abseil-string-find-str-contains," # most abseil warnings have been useful even without the lib, not this one though
     # "altera-*," # not writing for an FPGA here so only here for posterity
     "android-*,"
     "boost-*,"
     "bugprone-*,"
     "cert-*,"
     "-cert-err58-cpp," # if there is a thrown exception while initting static storage members, I want a throw
     "clang-*,"
     "concurency-*,"
     "cppcoreguidelines-*,"
     "-cppcoreguidelines-macro-usage," # nolint is not killing complaints about my warning silencing macros, so screw it
     "-cppcoreguidelines-avoid-c-arrays," # c-style arrays are too useful for purposes of having a pointer
     "-cppcoreguidelines-owning-memory," # I am NOT using the core guidelines gsl
     "-cppcoreguidelines-pro-bounds-pointer-arithmetic," # pointers are numbers and sometimes you have to use them as such
     "-cppcoreguidelines-avoid-magic-numbers," # 5 is not a flipping magic number when it is a static index into an array
     "-cppcoreguidelines-pro-bounds-constant-array-index," # OMG indexing with an expression is fine wtf
     "-cppcoreguidelines-no-malloc," # if I want aligned memory I want malloc so screw this"
     "-cppcoreguidelines-non-private-member-variables-in-classes," # making classes have all private members is ridiculous, if a member is accessed trivially, screw getters and setters
     "-cppcoreguidelines-pro-bounds-array-to-pointer-decay," # ARRAYS __ARE__ POINTERS!!!!!!!!!!!!!!!!!!!!!!
     "-cppcoreguidelines-pro-type-reinterpret-cast," # avoid the hell out of it, but sometimes you _have_ to
     "darwin-*,"
     "fuchsia-*"
     "google-*,"
     "hicpp-*,"
     "-hicpp-avoid-c-arrays," # I see no simple way to force alignment of Non-C arrays, so screw this
     "-hicpp-no-array-decay," # look, arrays _are_ pointers, I will decay them if I want to
     "-hicpp-no-malloc," # sometimes you _have_ to manage memory manually, especially with aligned allocation
     "linuxkernel-*,"
     "llvm-*,"
     "-llvm-include-order," # I have my own include ordering that I prefer
     "-llvm-header-guard," # I have my own header guard style that I prefer
     "misc-*,"
     "-misc-non-private-member-variables-in-classes," # I will not be screamed at for having structs for mass freight of data, but they have a "reset" member function
     "modernize-*,"
     "-modernize-avoid-c-arrays,"  # I see no simple way to force alignment of Non-C arrays, so screw this
     "-modernize-use-trailing-return-type," # I personally dislike this style because I visually search by return type, then by function name
     # "mpi-*," # not using MPI here so this is only in the list for posterity that the options are available
     "performance-*,"
     "portability-*,"
     "zircon-*")

list(JOIN CLANG_TIDY_CHECKS_LIST "" CLANG_TIDY_CHECKS)

list(APPEND RUN_CLANG_TIDY_BIN_ARGS
         -clang-tidy-binary ${CLANG_TIDY_EXE}
         -header-filter="${CMAKE_CURRENT_SOURCE_DIR}"
         -checks=${CLANG_TIDY_CHECKS})

add_custom_target(tidy
                  COMMAND ${RUN_CLANG_TIDY_PYTHON} ${RUN_CLANG_TIDY_BIN_ARGS}
                  COMMENT "Running `clang-tidy`")
"""

TEMPLATES["cmake_build_options"] =\
"""# Set list of build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;Warn;WarnWithOpt")

# Lists of flags for various build types
if (CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
  if (CMAKE_SYSTEM_PROCESSOR MATCHES arm64)
    set(RELEASE_FLAGS "-O3 -mtune=native -mcpu=apple-m1 -DNDEBUG")
  else()
    set(RELEASE_FLAGS "-O3 -mtune=native -march=native -DNDEBUG")
  endif()
else()
    set(RELEASE_FLAGS "-O3 -mtune=native -march=native -DNDEBUG")
endif()
#set(RELEASE_FLAGS "-O3 -mtune=native -march=native -DNDEBUG")
#set(RELEASE_FLAGS "-O3 -mtune=native -march=native")
set(DEBUG_FLAG "-g")
set(WARN_FLAGS_CLANG "-Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic")
#set(WARN_FLAGS_GCC "-Wall -Wextra -Wpedantic -Weffc++ -Wno-c++98-compat -Wno-c++98-compat-pedantic")
set(WARN_FLAGS_GCC "-Wall -Wextra -Wpedantic -Wno-c++98-compat -Wno-c++98-compat-pedantic")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    set(WARN_FLAGS ${WARN_FLAGS_CLANG})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    # using Clang
    set(WARN_FLAGS ${WARN_FLAGS_CLANG})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using gcc
    set(WARN_FLAGS ${WARN_FLAGS_GCC})
else()
    # Unsupported compiler
    # If you are are windows, whelp... I got 99 problems but M$ ain't one of them
    # If you are using the Intel compiler, well... that decision is between you and your deity
    set(WARN_FLAGS "")
    message(SEND_ERROR "Compilers other than `clang` and `gcc` are not supported.")
endif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

###########################################################
##  Setup Flag To Tell Code The Compiler (Simplifying Checks)
###########################################################
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    set(COMPILER_NAME_FLAG "-DCLANG_COMPILER=1")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    # using Clang
    set(COMPILER_NAME_FLAG "-DCLANG_COMPILER=1")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(COMPILER_NAME_FLAG "-DGCC_COMPILER=1")
else()
    # Unsupported compiler
endif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

# Handle if the user set things to use a sanitizer
if(SANITIZER STREQUAL "Address")
    set(SANITIZER_CXX_FLAGS  "-fsanitize=address -DBOOST_USE_ASAN=1")
    set(SANITIZER_LINKER_FLAGS  "-fsanitize=address")
elseif(SANITIZER STREQUAL "Thread")
    set(SANITIZER_CXX_FLAGS  "-fsanitize=thread")
    set(SANITIZER_LINKER_FLAGS  "-fsanitize=thread")
elseif(SANITIZER STREQUAL "Memory")
    set(SANITIZER_CXX_FLAGS  "-fsanitize=memory")
    set(SANITIZER_LINKER_FLAGS  "-fsanitize=memory")
elseif(SANITIZER STREQUAL "UndefinedBehavior")
    set(SANITIZER_CXX_FLAGS  "-fsanitize=undefined")
    set(SANITIZER_LINKER_FLAGS  "-fsanitize=undefined")
elseif(SANITIZER STREQUAL "Leak")
    set(SANITIZER_CXX_FLAGS  "-fsanitize=leak")
    set(SANITIZER_LINKER_FLAGS  "-fsanitize=leak")
endif(SANITIZER STREQUAL "Address")

# set up for the plain build type
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${COMPILER_NAME_FLAG} ${SANITIZER_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZER_LINKER_FLAGS}")
# set up debug flags
set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS} ${COMPILER_NAME_FLAG} ${DEBUG_FLAG} ${SANITIZER_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG  "${CMAKE_EXE_LINKER_FLAGS} ${DEBUG_FLAG} ${SANITIZER_LINKER_FLAGS}")
# set up release flags
set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS} ${COMPILER_NAME_FLAG} ${RELEASE_FLAGS} ${SANITIZER_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE  "${CMAKE_EXE_LINKER_FLAGS} ${RELEASE_FLAGS} ${SANITIZER_LINKER_FLAGS}")
# set up release with debug flags
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS} ${COMPILER_NAME_FLAG} ${DEBUG_FLAG} ${RELEASE_FLAGS} ${SANITIZER_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO  "${CMAKE_EXE_LINKER_FLAGS} ${DEBUG_FLAG} ${RELEASE_FLAGS} ${SANITIZER_LINKER_FLAGS}")
# set up warn flags
set(CMAKE_CXX_FLAGS_WARN  "${CMAKE_CXX_FLAGS} ${COMPILER_NAME_FLAG} ${WARN_FLAGS} ${SANITIZER_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_WARN  "${CMAKE_EXE_LINKER_FLAGS} ${WARN_FLAGS} ${SANITIZER_LINKER_FLAGS}")
# set up warn with optimization flags
set(CMAKE_CXX_FLAGS_WARNWITHOPT  "${CMAKE_CXX_FLAGS} ${COMPILER_NAME_FLAG} ${RELEASE_FLAGS} ${WARN_FLAGS} ${SANITIZER_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_WARNWITHOPT  "${CMAKE_EXE_LINKER_FLAGS} ${RELEASE_FLAGS} ${WARN_FLAGS} ${SANITIZER_LINKER_FLAGS}")
"""

TEMPLATES["base_dir_dirdata"] =\
"""# Get Directory data from every sub directory
include(./src/DirData.cmake)


# Make the full list of all sources
list(APPEND {project_name_cap:s}_SRCS
     ${{SRC_SRCS}}
     )
# Make the full list of all the headers
list(APPEND {project_name_cap:s}_HDRS
     ${{SRC_HDRS}}
     )
"""

TEMPLATES["src_dir_dirdata"] =\
"""# pull the subdirectory information if it is needed

# Source files from this directory
list(APPEND SRC_SRCS
     ./src/main.cpp)

# Header files from this directory
list(APPEND SRC_HDRS )

"""

TEMPLATES["build_config_header_input"] =\
"""/***************************************************************************//**
********************************************************************************
**
** @author {author_name:s}
** @date {day:s} {month_abbrev:s}, {year:s}
**
** @copyright Copyright (C) {year:s} {copyright_holder:s}
**
********************************************************************************
*******************************************************************************/
#ifndef {macro_guard_name:s}
#define {macro_guard_name:s}

#define {project_name_cap:s}_VERSION "@{project_name_cap:s}_VERSION@"
#define {project_name_cap:s}_VERSION_MAJOR @{project_name_cap:s}_VERSION_MAJOR@
#define {project_name_cap:s}_VERSION_MINOR @{project_name_cap:s}_VERSION_MINOR@
#define {project_name_cap:s}_VERSION_PATCH @{project_name_cap:s}_VERSION_PATCH@
#define {project_name_cap:s}_BUILD "@BUILD_TYPE@"
#define {project_name_cap:s}_COMMIT_SHA "@{project_name_cap:s}_VERSION_SHA@"

#endif // {macro_guard_name:s}
"""

TEMPLATES["bottom_level_makefile"] =\
"""#This variable stores the name/path of the directory with all the object files will be (relative to the make file)
OBJ_DIR=build
SRC_DIR={project_name:s}
CMAKE_CMD=cmake
MAKE_CMD=make

ifneq ($(SANI),)
CMAKE_ADD_SANI="-DSANITIZER=$(SANI)"
DIR_APPEND="-$(SANI)"
else
CMAKE_ADD_SANI=
DIR_APPEND=
endif

CMAKE_GEN_CMD=-DCMAKE_INSTALL_PREFIX=$(shell pwd) $(CMAKE_ADD_SANI)
SYSNAME=$(shell hostname)

#this builds the project with no flags but those absolutely necessary
.PHONY: plain
plain: BUILD_DIR=$(OBJ_DIR)/plain$(DIR_APPEND)
plain: CMAKE_FLAGS=-DCMAKE_BUILD_TYPE="" $(CMAKE_GEN_CMD)
plain: primary_build_rule

#this builds the project with debug flags making it easier to attach a debugger and see what is happening
.PHONY: debug
debug: BUILD_DIR=$(OBJ_DIR)/Debug$(DIR_APPEND)
debug: CMAKE_FLAGS=-DCMAKE_BUILD_TYPE=Debug $(CMAKE_GEN_CMD)
debug: primary_build_rule

#this builds the project with debug flags and with optimization flags allowing you to attach a debugger
#to see if the optimization is screwing something up
.PHONY: opt_debug
opt_debug: BUILD_DIR=$(OBJ_DIR)/RelWithDebInfo$(DIR_APPEND)
opt_debug: CMAKE_FLAGS=-DCMAKE_BUILD_TYPE=RelWithDebInfo $(CMAKE_GEN_CMD)
opt_debug: primary_build_rule

#this builds the project with no debug flags and with optimization flags making it run faster
.PHONY: release
release: BUILD_DIR=$(OBJ_DIR)/Release$(DIR_APPEND)
release: CMAKE_FLAGS=-DCMAKE_BUILD_TYPE=Release $(CMAKE_GEN_CMD)
release: primary_build_rule

#this builds the project with the generic warning flags enabled to see where you might want to make changed
.PHONY: warn
warn: BUILD_DIR=$(OBJ_DIR)/Warn$(DIR_APPEND)
warn: CMAKE_FLAGS=-DCMAKE_BUILD_TYPE=Warn $(CMAKE_GEN_CMD)
warn: primary_build_rule

#this builds the project with the generic warning flags enabled to see where you might want to make changed
.PHONY: warn_opt
warn_opt: BUILD_DIR=$(OBJ_DIR)/WarnWithOpt$(DIR_APPEND)
warn_opt: CMAKE_FLAGS=-DCMAKE_BUILD_TYPE=WarnWithOpt $(CMAKE_GEN_CMD)
warn_opt: primary_build_rule

#this runs the clang-tidy rule on the project, but does not build
.PHONY: tidy
tidy: BUILD_DIR=$(OBJ_DIR)/tidy
tidy: CMAKE_FLAGS=-DCMAKE_BUILD_TYPE="" $(CMAKE_GEN_CMD)
tidy:
\t@mkdir -p $(BUILD_DIR)
\t@cd $(BUILD_DIR); $(CMAKE_CMD) $(CMAKE_FLAGS) ../../$(SRC_DIR)
\t@echo "Hitting clang tidy checks"
\t@$(MAKE) -C $(BUILD_DIR) tidy

.PHONY: primary_build_rule
primary_build_rule:
\t@mkdir -p $(BUILD_DIR)
\t@cd $(BUILD_DIR); $(CMAKE_CMD) $(CMAKE_FLAGS) ../../$(SRC_DIR)
\t@$(MAKE) -C $(BUILD_DIR)
\t@$(MAKE) -C $(BUILD_DIR) install
ifneq ($(SANI),)
\tdsymutil $(EXE_FILE)
endif
ifeq ($(SYSNAME),champ)
\t@mkdir -p logs
\t@find logs -type d -print0 | xargs -0 chmod g+w
\t@find build -type d -print0 | xargs -0 chmod g+w
endif

.PHONY: help
help:
\t@echo ''
\t@echo 'Build Types:'
\t@echo '  `plain`     - Build with no flags but those strictly necessary'
\t@echo '  `debug`     - Build with debug flag'
\t@echo '  `opt_debug` - Build with debug and optimization flags'
\t@echo '  `release`   - Build with optimization flags'
\t@echo '  `warn`      - Build with all the warning flags we can find'
\t@echo '  `warn_opt`  - Build with all the warning flags and release flags (enables a few more warnings)'
\t@echo ''
\t@echo 'Sanitizers:'
\t@echo '  To use a clang sanitizer, add `SANI=$$(Option)` to your command line'
\t@echo '  The sanitizer options are:'
\t@echo '    `Address`           - Finds out of bounds accesses (to heap, stack and globals), use-after-free, and double-free ~2x slowdown'
\t@echo '    `Thread`            - Detects data races (with limitations) 5-15x slow down, 5-10x memory overhead'
\t@echo '    `Memory`            - Finds uninitialized reads ~3x slowdown'
\t@echo '    `UndefinedBehavior` - Finds undifined behavior almost no slowdown (has further options, see doc)'
\t@echo '    `Leak`              - Finds memory leaks, almost no slowdown until the end of the process'
\t@echo '  More about sanitizers at: https://releases.llvm.org/6.0.0/tools/clang/docs/'
\t@echo ''
\t@echo 'Miscellaneous Options:'
\t@echo '  To build multiple files in parallel use'
\t@echo '    `-j$$(NUM_BUILD)`'
\t@echo '  To see the commands issued during the buld process add the following to the command line'
\t@echo '    `VERBOSE=1`'
\t@echo ''
\t@echo 'Example:'
\t@echo '  To show the commands run, run 8 build commands simultaneously,'
\t@echo '  use the address sanitizer, and have optimization and debug symbols you would run'
\t@echo '    `make -j 8 VERBOSE=1 SANI=Address opt_debug`'
\t@echo ''

#cleaning targets to remove various things generated by this make file
#this removes the contents of the build directories
.PHONY: clean
clean:
\t-rm -rf $(OBJ_DIR)

#this runs clean and then removes the executable
.PHONY: cleanall
cleanall: clean
\t-rm -f {exec_name:s}
"""

TEMPLATES["clang_tidy_python_script"] =\
"""#!/usr/bin/env python3
#
#===- run-clang-tidy.py - Parallel clang-tidy runner --------*- python -*--===#
#
# Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
#===-----------------------------------------------------------------------===#
# FIXME: Integrate with clang-tidy-diff.py


\"""
Parallel clang-tidy runner
==========================

Runs clang-tidy over all files in a compilation database. Requires clang-tidy
and clang-apply-replacements in $PATH.

Example invocations.
- Run clang-tidy on all files in the current working directory with a default
  set of checks and show warnings in the cpp files and all project headers.
    run-clang-tidy.py $PWD

- Fix all header guards.
    run-clang-tidy.py -fix -checks=-*,llvm-header-guard

- Fix all header guards included from clang-tidy and header guards
  for clang-tidy headers.
    run-clang-tidy.py -fix -checks=-*,llvm-header-guard extra/clang-tidy \
                      -header-filter=extra/clang-tidy

Compilation database setup:
http://clang.llvm.org/docs/HowToSetupToolingForLLVM.html
\"""

from __future__ import print_function

import argparse
import glob
import json
import multiprocessing
import os
import re
import shutil
import subprocess
import sys
import tempfile
import threading
import traceback

try:
  import yaml
except ImportError:
  yaml = None

is_py2 = sys.version[0] == '2'

if is_py2:
    import Queue as queue
else:
    import queue as queue


def find_compilation_database(path):
  \"""Adjusts the directory until a compilation database is found.\"""
  result = './'
  while not os.path.isfile(os.path.join(result, path)):
    if os.path.realpath(result) == '/':
      print('Error: could not find compilation database.')
      sys.exit(1)
    result += '../'
  return os.path.realpath(result)


def make_absolute(f, directory):
  if os.path.isabs(f):
    return f
  return os.path.normpath(os.path.join(directory, f))


def get_tidy_invocation(f, clang_tidy_binary, checks, tmpdir, build_path,
                        header_filter, line_filter,
                        allow_enabling_alpha_checkers,
                        extra_arg, extra_arg_before, quiet, config):
  \"""Gets a command line for clang-tidy.\"""
  start = [clang_tidy_binary, '--use-color']
  if allow_enabling_alpha_checkers:
    start.append('-allow-enabling-analyzer-alpha-checkers')
  if header_filter is not None:
    start.append('-header-filter=' + header_filter)
  if line_filter is not None:
    start.append('--line-filter=' + line_filter)
  if checks:
    start.append('-checks=' + checks)
  if tmpdir is not None:
    start.append('-export-fixes')
    # Get a temporary file. We immediately close the handle so clang-tidy can
    # overwrite it.
    (handle, name) = tempfile.mkstemp(suffix='.yaml', dir=tmpdir)
    os.close(handle)
    start.append(name)
  for arg in extra_arg:
      start.append('-extra-arg=%s' % arg)
  for arg in extra_arg_before:
      start.append('-extra-arg-before=%s' % arg)
  start.append('-p=' + build_path)
  if quiet:
      start.append('-quiet')
  if config:
      start.append('-config=' + config)
  start.append(f)
  return start


def merge_replacement_files(tmpdir, mergefile):
  \"""Merge all replacement files in a directory into a single file\"""
  # The fixes suggested by clang-tidy >= 4.0.0 are given under
  # the top level key 'Diagnostics' in the output yaml files
  mergekey = "Diagnostics"
  merged=[]
  for replacefile in glob.iglob(os.path.join(tmpdir, '*.yaml')):
    content = yaml.safe_load(open(replacefile, 'r'))
    if not content:
      continue # Skip empty files.
    merged.extend(content.get(mergekey, []))

  if merged:
    # MainSourceFile: The key is required by the definition inside
    # include/clang/Tooling/ReplacementsYaml.h, but the value
    # is actually never used inside clang-apply-replacements,
    # so we set it to '' here.
    output = {'MainSourceFile': '', mergekey: merged}
    with open(mergefile, 'w') as out:
      yaml.safe_dump(output, out)
  else:
    # Empty the file:
    open(mergefile, 'w').close()


def check_clang_apply_replacements_binary(args):
  \"""Checks if invoking supplied clang-apply-replacements binary works.\"""
  try:
    subprocess.check_call([args.clang_apply_replacements_binary, '--version'])
  except:
    print('Unable to run clang-apply-replacements. Is clang-apply-replacements '
          'binary correctly specified?', file=sys.stderr)
    traceback.print_exc()
    sys.exit(1)


def apply_fixes(args, tmpdir):
  \"""Calls clang-apply-fixes on a given directory.\"""
  invocation = [args.clang_apply_replacements_binary]
  if args.format:
    invocation.append('-format')
  if args.style:
    invocation.append('-style=' + args.style)
  invocation.append(tmpdir)
  subprocess.call(invocation)


def run_tidy(args, tmpdir, build_path, queue, lock, failed_files):
  \"""Takes filenames out of queue and runs clang-tidy on them.\"""
  while True:
    name = queue.get()
    invocation = get_tidy_invocation(name, args.clang_tidy_binary, args.checks,
                                     tmpdir, build_path, args.header_filter,
                                     args.line_filter,
                                     args.allow_enabling_alpha_checkers,
                                     args.extra_arg, args.extra_arg_before,
                                     args.quiet, args.config)

    proc = subprocess.Popen(invocation, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    output, err = proc.communicate()
    if proc.returncode != 0:
      failed_files.append(name)
    with lock:
      sys.stdout.write(' '.join(invocation) + '\\n' + output.decode('utf-8'))
      if len(err) > 0:
        sys.stdout.flush()
        sys.stderr.write(err.decode('utf-8'))
    queue.task_done()


def main():
  parser = argparse.ArgumentParser(description='Runs clang-tidy over all files '
                                   'in a compilation database. Requires '
                                   'clang-tidy and clang-apply-replacements in '
                                   '$PATH.')
  parser.add_argument('-allow-enabling-alpha-checkers',
                      action='store_true', help='allow alpha checkers from '
                                                'clang-analyzer.')
  parser.add_argument('-clang-tidy-binary', metavar='PATH',
                      default='clang-tidy',
                      help='path to clang-tidy binary')
  parser.add_argument('-clang-apply-replacements-binary', metavar='PATH',
                      default='clang-apply-replacements',
                      help='path to clang-apply-replacements binary')
  parser.add_argument('-checks', default=None,
                      help='checks filter, when not specified, use clang-tidy '
                      'default')
  parser.add_argument('-config', default=None,
                      help='Specifies a configuration in YAML/JSON format: '
                      '  -config="{Checks: \\'*\\', '
                      '                       CheckOptions: [{key: x, '
                      '                                       value: y}]}" '
                      'When the value is empty, clang-tidy will '
                      'attempt to find a file named .clang-tidy for '
                      'each source file in its parent directories.')
  parser.add_argument('-header-filter', default=None,
                      help='regular expression matching the names of the '
                      'headers to output diagnostics from. Diagnostics from '
                      'the main file of each translation unit are always '
                      'displayed.')
  parser.add_argument('-line-filter', default=None,
                      help='json object array describing ranges of lines to '
                      'ignore from specified files. See man clang-tidy for '
                      'more')
  if yaml:
    parser.add_argument('-export-fixes', metavar='filename', dest='export_fixes',
                        help='Create a yaml file to store suggested fixes in, '
                        'which can be applied with clang-apply-replacements.')
  parser.add_argument('-j', type=int, default=0,
                      help='number of tidy instances to be run in parallel.')
  parser.add_argument('files', nargs='*', default=['.*'],
                      help='files to be processed (regex on path)')
  parser.add_argument('-fix', action='store_true', help='apply fix-its')
  parser.add_argument('-format', action='store_true', help='Reformat code '
                      'after applying fixes')
  parser.add_argument('-style', default='file', help='The style of reformat '
                      'code after applying fixes')
  parser.add_argument('-p', dest='build_path',
                      help='Path used to read a compile command database.')
  parser.add_argument('-extra-arg', dest='extra_arg',
                      action='append', default=[],
                      help='Additional argument to append to the compiler '
                      'command line.')
  parser.add_argument('-extra-arg-before', dest='extra_arg_before',
                      action='append', default=[],
                      help='Additional argument to prepend to the compiler '
                      'command line.')
  parser.add_argument('-quiet', action='store_true',
                      help='Run clang-tidy in quiet mode')
  args = parser.parse_args()

  db_path = 'compile_commands.json'

  if args.build_path is not None:
    build_path = args.build_path
  else:
    # Find our database
    build_path = find_compilation_database(db_path)

  try:
    invocation = [args.clang_tidy_binary, '-list-checks']
    if args.allow_enabling_alpha_checkers:
      invocation.append('-allow-enabling-analyzer-alpha-checkers')
    invocation.append('-p=' + build_path)
    if args.checks:
      invocation.append('-checks=' + args.checks)
    invocation.append('-')
    # We want to check if we can call clang-tidy not dump a list of checks
    with open(os.devnull, 'w') as dev_null:
      subprocess.check_call(invocation, stdout=dev_null)
  except:
    print("Unable to run clang-tidy.", file=sys.stderr)
    sys.exit(1)

  # Load the database and extract all files.
  database = json.load(open(os.path.join(build_path, db_path)))
  files = [make_absolute(entry['file'], entry['directory'])
           for entry in database]

  max_task = args.j
  if max_task == 0:
    max_task = multiprocessing.cpu_count()

  tmpdir = None
  if args.fix or (yaml and args.export_fixes):
    check_clang_apply_replacements_binary(args)
    tmpdir = tempfile.mkdtemp()

  # Build up a big regexy filter from all command line arguments.
  file_name_re = re.compile('|'.join(args.files))

  return_code = 0
  try:
    # Spin up a bunch of tidy-launching threads.
    task_queue = queue.Queue(max_task)
    # List of files with a non-zero return code.
    failed_files = []
    lock = threading.Lock()
    for _ in range(max_task):
      t = threading.Thread(target=run_tidy,
                           args=(args, tmpdir, build_path, task_queue, lock, failed_files))
      t.daemon = True
      t.start()

    # Fill the queue with files.
    for name in files:
      if file_name_re.search(name):
        task_queue.put(name)

    # Wait for all threads to be done.
    task_queue.join()
    if len(failed_files):
      return_code = 1

  except KeyboardInterrupt:
    # This is a sad hack. Unfortunately subprocess goes
    # bonkers with ctrl-c and we start forking merrily.
    print('\\nCtrl-C detected, goodbye.')
    if tmpdir:
      shutil.rmtree(tmpdir)
    os.kill(0, 9)

  if yaml and args.export_fixes:
    print('Writing fixes to ' + args.export_fixes + ' ...')
    try:
      merge_replacement_files(tmpdir, args.export_fixes)
    except:
      print('Error exporting fixes.\\n', file=sys.stderr)
      traceback.print_exc()
      return_code=1

  if args.fix:
    print('Applying fixes ...')
    try:
      apply_fixes(args, tmpdir)
    except:
      print('Error applying fixes.\\n', file=sys.stderr)
      traceback.print_exc()
      return_code = 1

  if tmpdir:
    shutil.rmtree(tmpdir)
  sys.exit(return_code)


if __name__ == '__main__':
  main()
"""
