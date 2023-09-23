# pull the subdirectory information if it is needed
include(./src/Parameters/DirData.cmake)

# Source files from this directory
list(APPEND SRC_SRCS
     ${DGSSORT_SRC_PARAMETERS_SRCS}
     ./src/main.cpp)

# Header files from this directory
list(APPEND SRC_HDRS
     ${DGSSORT_SRC_PARAMETERS_HDRS} )

