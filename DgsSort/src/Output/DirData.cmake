# pull the subdirectory information if it is needed

# Source files from this directory
list(APPEND DGSSORT_SRC_OUTPUT_SRCS 
     ./src/Output/HistWriter.cpp)

# Header files from this directory
list(APPEND DGSSORT_SRC_OUTPUT_HDRS
     ./src/Output/BinStructs.h 
     ./src/Output/HistWriter.h)
