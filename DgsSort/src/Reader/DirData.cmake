# pull the subdirectory information if it is needed

# Source files from this directory
list(APPEND DGSSORT_SRC_READER_SRCS
     ./src/Reader/Calibrator.cpp
     ./src/Reader/DgsReader.cpp )

# Header files from this directory
list(APPEND DGSSORT_SRC_READER_HDRS
     ./src/Reader/Calibrator.h 
     ./src/Reader/DgsReader.h)
