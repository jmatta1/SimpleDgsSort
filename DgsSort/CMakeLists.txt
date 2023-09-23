cmake_minimum_required(VERSION 3.1)

###########################################################
##  Setup Project
###########################################################
project(DgsSort)

execute_process(COMMAND "git" "rev-parse" "-q" "HEAD" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE DGSSORT_VERSION_SHA)

# The version number.
set(DGSSORT_VERSION_MAJOR 0)
set(DGSSORT_VERSION_MINOR 0)
set(DGSSORT_VERSION_PATCH 0)
set(DGSSORT_VERSION ${DGSSORT_VERSION_MAJOR}.${DGSSORT_VERSION_MINOR}.${DGSSORT_VERSION_PATCH})

message(STATUS "Version is: " ${DGSSORT_VERSION})

message(STATUS "Git revision is: " ${DGSSORT_VERSION_SHA})
message(STATUS "Src dir is: " ${CMAKE_CURRENT_SOURCE_DIR})

# Set the version number and build type in the header /Utility/OrchidConfig.h
configure_file("./src/BuildConfig/DgsSortBuildConfig.h.in"
               "${PROJECT_BINARY_DIR}/src/BuildConfig/DgsSortBuildConfig.h"
               @ONLY)

###########################################################
##  Setup C++ options
###########################################################
# set for c++ 17
set(CMAKE_CXX_STANDARD 17)
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
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")


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
include_directories("${PROJECT_BINARY_DIR}/src/BuildConfig")

###########################################################
##  Grab all the sub-directory file data
###########################################################
include(./DirData.cmake)
list(APPEND SRCS ${DGSSORT_SRCS})
list(APPEND HDRS ${DGSSORT_HDRS})

###########################################################
##  Override Source Groups
###########################################################
# So things look nice in QtCreator
SOURCE_GROUP("" FILES ${SRCS} ${HDRS})

###########################################################
##  Target Setup
###########################################################
# Add the executable and link it up
add_executable(dgsSort ${SRCS} ${HDRS})

if(CLANG_TIDY_EXE)
  set_target_properties(dgsSort PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
endif()

###########################################################
##  Install setup
###########################################################
get_filename_component(ABS_BIN_INSTALL_DIR "./.." ABSOLUTE)
install(TARGETS dgsSort RUNTIME DESTINATION ${ABS_BIN_INSTALL_DIR})

