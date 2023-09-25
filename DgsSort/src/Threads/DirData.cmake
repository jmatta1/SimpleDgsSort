# pull the subdirectory information if it is needed

# Source files from this directory
list(APPEND DGSSORT_SRC_THREADS_SRCS 
     ./src/Threads/ReaderThread.cpp)

# Header files from this directory
list(APPEND DGSSORT_SRC_THREADS_HDRS
     ./src/Threads/BufferQueue.h 
     ./src/Threads/ReaderThread.h)
