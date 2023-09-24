/***************************************************************************//**
********************************************************************************
**
** @author James Till Matta, and Akaa Daniel Ayageakaa et al
** @date 23 Sep, 2023
**
** @copyright Copyright (C) 2023 Oak Ridge National Laboratory
**
********************************************************************************
*******************************************************************************/
// includes for C system headers
// includes for C++ system headers
#include<iostream>
// includes from other libraries
// includes from DgsSort
#include"DgsSortBuildConfig.h"

int main(int argc, char* argv[])
{
    std::cout << "\nDgsSort\nDigital GammaSphere Sorter"
              << "\n     Version: " << DGSSORT_VERSION
              << "\n  Build Type: " << DGSSORT_BUILD
              << "\n  Git Commit: " << DGSSORT_COMMIT_SHA << std::endl;


    return 0;
}
