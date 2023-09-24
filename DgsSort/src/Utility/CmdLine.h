/***************************************************************************//**
********************************************************************************
**
** @author James Till Matta, and Akaa Daniel Ayageakaa et al
** @date 24 Sep, 2023
**
** @copyright Copyright (C) 2023 ??
**
********************************************************************************
*******************************************************************************/
#ifndef DGSSORT_SRC_UTILITY_CMDLINE_H
#define DGSSORT_SRC_UTILITY_CMDLINE_H

// includes for C system headers
// includes for C++ system headers
#include<string>
// includes from other libraries
// includes from DgsSort

namespace Utility
{

struct CmdLineOpts
{
    std::string dgsFileName{""};
    std::string rootFileName{""};
    std::string rootFileOptions{""};
    std::string calFileName{""};
    double recoilBeta{0.0};
};

bool parseCommandLine(int argc, char* argv[], CmdLineOpts& clo);

} // namespace Utility

#endif  // DGSSORT_SRC_UTILITY_CMDLINE_H
