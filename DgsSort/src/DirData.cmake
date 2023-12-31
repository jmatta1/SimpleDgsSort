# pull the subdirectory information if it is needed
include(./src/Threads/DirData.cmake)
include(./src/Output/DirData.cmake)
include(./src/Utility/DirData.cmake)
include(./src/Reader/DirData.cmake)
include(./src/Parameters/DirData.cmake)

# Source files from this directory
list(APPEND SRC_SRCS
     ${DGSSORT_SRC_THREADS_SRCS}
     ${DGSSORT_SRC_OUTPUT_SRCS}
     ${DGSSORT_SRC_UTILITY_SRCS}
     ${DGSSORT_SRC_READER_SRCS}
     ${DGSSORT_SRC_PARAMETERS_SRCS}
     ./src/main.cpp)

# Header files from this directory
list(APPEND SRC_HDRS
     ${DGSSORT_SRC_THREADS_HDRS}
     ${DGSSORT_SRC_OUTPUT_HDRS}
     ${DGSSORT_SRC_UTILITY_HDRS}
     ${DGSSORT_SRC_READER_HDRS}
     ${DGSSORT_SRC_PARAMETERS_HDRS} )

