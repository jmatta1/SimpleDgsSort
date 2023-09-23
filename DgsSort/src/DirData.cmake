# pull the subdirectory information if it is needed
include(./src/Utility/DirData.cmake)
include(./src/Reader/DirData.cmake)
include(./src/Parameters/DirData.cmake)

# Source files from this directory
list(APPEND SRC_SRCS
     ${DGSSORT_SRC_UTILITY_SRCS}
     ${DGSSORT_SRC_READER_SRCS}
     ${DGSSORT_SRC_PARAMETERS_SRCS}
     ./src/main.cpp)

# Header files from this directory
list(APPEND SRC_HDRS
     ${DGSSORT_SRC_UTILITY_HDRS}
     ${DGSSORT_SRC_READER_HDRS}
     ${DGSSORT_SRC_PARAMETERS_HDRS} )

