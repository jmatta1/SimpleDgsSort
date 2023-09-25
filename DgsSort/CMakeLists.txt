cmake_minimum_required(VERSION 3.20)

###########################################################
##  Setup Project
###########################################################
project(DgsSort)

execute_process(COMMAND "git" "rev-parse" "-q" "HEAD" WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE DGSSORT_VERSION_SHA RESULT_VARIABLE RETVAL)

if(NOT RETVAL EQUAL "0")
  message(STATUS "The fatal error from git is not actually a problem, don't worry")
endif(NOT RETVAL EQUAL "0")


# The version number.
set(DGSSORT_VERSION_MAJOR 0)
set(DGSSORT_VERSION_MINOR 5)
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
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/")
# add the root dependency
find_package(ROOT REQUIRED COMPONENTS RIO Core Tree)
message(STATUS "Found ROOT use file at: ${ROOT_USE_FILE}")
include(${ROOT_USE_FILE})
# add the zlib dependency
find_package(ZLIB)

###########################################################
##  Setup Build Options
###########################################################
include(./cmake/BldOpt.cmake)

###########################################################
##  Setup Source Analysis Helpers Like Clang-Tidy
###########################################################
include(./cmake/Analyzers.cmake)


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
source_group("" FILES ${SRCS} ${HDRS})

###########################################################
##  Target Creation
###########################################################
# Add the executable and link it up
add_executable(dgSort ${SRCS} ${HDRS})

###########################################################
##  Setup include directories
###########################################################
# Add this root directory as an include directory so that we can reference
# our include files relative to it (less ../../../ business)
target_include_directories(dgSort PRIVATE ${PROJECT_SOURCE_DIR}/src)
# add the root include directories
target_include_directories(dgSort SYSTEM PRIVATE ROOT::Core ROOT::RIO ROOT::Tree)
# now add the external library directory so that we can use the pcg prng
target_include_directories(dgSort SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/ExtLib)
# Add the generated file header directory in this way to allow files in the
# utility directory to access the generated file as if it was in their directory
target_include_directories(dgSort PRIVATE "${PROJECT_BINARY_DIR}/src/BuildConfig")


###########################################################
##  Target Linking
###########################################################
# link in ROOT
target_link_libraries(dgSort ROOT::Core ROOT::RIO ROOT::Hist ROOT::Tree)
# link in zlib
target_link_libraries(dgSort ZLIB::ZLIB)


if(CLANG_TIDY_EXE)
  set_target_properties(dgSort PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
endif()

###########################################################
##  Install setup
###########################################################
get_filename_component(ABS_BIN_INSTALL_DIR "./.." ABSOLUTE)
install(TARGETS dgSort RUNTIME DESTINATION ${ABS_BIN_INSTALL_DIR})
