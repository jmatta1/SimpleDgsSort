# Get Directory data from every sub directory
include(./src/DirData.cmake)


# Make the full list of all sources
list(APPEND DGSSORT_SRCS
     ${SRC_SRCS}
     )
# Make the full list of all the headers
list(APPEND DGSSORT_HDRS
     ${SRC_HDRS}
     )
