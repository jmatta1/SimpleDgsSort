/***************************************************************************//**
********************************************************************************
**
** @author James Till Matta
** @date 24 Sep, 2023
**
** @copyright Copyright (C) 2023 James Till Matta
**
********************************************************************************
*******************************************************************************/
#include"CmdLine.h"
// includes for C system headers
// includes for C++ system headers
#include<iostream>
#include<filesystem>
#include<cstring>
// includes from other libraries
// includes from DgsSort

namespace Utility
{

bool parseCommandLine(int argc, char* argv[], CmdLineOpts& clo)
{
    if(argc != 6)
    {
        std::cout << "Wrong number of arguments, got: " << argc - 1
                  << "expected: 5\n";
        std::cout << "Usage:\n    " << argv[0] << " <DgsFileName> <RootFileName> "
                  << "<RootFileOpenOption> <CalFileName> <RecoillBeta>\n" << std::flush;
        return false;
    }

    // pull in the arguments
    std::filesystem::path rawPath{argv[1]};
    std::filesystem::path rootPath{argv[2]};
    clo.rootFileOptions = argv[3];
    std::filesystem::path calPath{argv[4]};
    size_t charsProcessed{0};
    clo.recoilBeta = std::stod(argv[5], &charsProcessed);

    // make sure that the raw data path exists
    if(!std::filesystem::exists(rawPath))
    {
        std::cout << "The raw data input file: '" << rawPath << "' does not exist\n"<<std::flush;
        return false;
    }

    clo.dgsFileName = rawPath.native();

    //make sure the options are all upper case
    for(auto& c : clo.rootFileOptions)
    {
        c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    }

    // ensure the root path exists or not depending on options
    bool rootExists = std::filesystem::exists(rootPath);
    if(clo.rootFileOptions == "CREATE" || clo.rootFileOptions == "NEW")
    {
        if(rootExists)
        {
            std::cout << "The root file option was marked 'CREATE' (or 'NEW')."
                      << "But the root file given: '" << rootPath
                      << "' already exists.\n" << std::flush;
            return false;
        }
        // otherwise we are fine
    }
    else if(clo.rootFileOptions == "RECREATE")
    {
        //if the option is set to recreate, just make sure that if rootPath exists it's not a directory
        if(rootExists && std::filesystem::is_directory(rootPath))
        {
            std::cout << "The root file option was marked 'RECREATE'"
                      << "But the root file given: '" << rootPath
                      << "' is an existing _directory_\n" << std::flush;
            return false;
        }
        // otherwise we are fine
    }
    else if(clo.rootFileOptions == "UPDATE")
    {
        // if the option is set to update ensure that root path exists
        if(!rootExists)
        {
            std::cout << "The root file option was marked 'UPDATE'"
                      << "But the root file given: '" << rootPath
                      << "' does not already exist.\n" << std::flush;
            return false;
        }
        if(std::filesystem::is_directory(rootPath))
        {
            std::cout << "The root file option was marked 'UPDATE'"
                      << "But the root file given: '" << rootPath
                      << "' is a directory.\n" << std::flush;
            return false;
        }
        //otherwise we are fine
    }
    else
    {
        std::cout << "Unrecognized root file opening option: '" << clo.rootFileOptions
                  << "'\n    The option must be one of: 'CREATE', 'RECREATE', or 'UPDATE'\n" << std::flush;
        return false;
    }

    clo.rootFileName = rootPath.native();

    // now check the cal file existence
    if(!std::filesystem::exists(calPath))
    {
        std::cout << "The calibration file given: '" << calPath
                  << "' does not exist!\n" << std::flush;
        return false;
    }
    if(std::filesystem::is_directory(calPath))
    {
        std::cout << "The calibration file given: '" << calPath
                  << "' exists but is a directory!\n" << std::flush;
        return false;
    }
    clo.calFileName = calPath.native();

    // make sure _something_ was processed to generate the recoil beta
    if(charsProcessed == 0)
    {
        std::cout << "Invalid value passed as <RecoilBeta>\n" << std::flush;
        return false;
    }

    if(clo.recoilBeta < 0.0 || clo.recoilBeta >= 1.0)
    {
        std::cout << "Recoil beta must fall into the range: 0.0 <= RecoilBeta < 1.0\n" << std::flush;
        return false;
    }

    // if we are here all the basic sanity checks are done

    return true;
}

} // namespace Utility

