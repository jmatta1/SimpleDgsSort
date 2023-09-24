# pull the subdirectory information if it is needed

# Source files from this directory
list(APPEND DGSSORT_SRC_UTILITY_SRCS
     ./src/Utility/Misc.cpp
     ./src/Utility/CmdLine.cpp )

# Header files from this directory
list(APPEND DGSSORT_SRC_UTILITY_HDRS
     ./src/Utility/Misc.h
     ./src/Utility/CmdLine.h 
     ./src/Utility/EndianHandling.h)
