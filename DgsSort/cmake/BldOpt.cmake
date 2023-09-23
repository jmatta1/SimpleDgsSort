# Set list of build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;Warn;WarnWithOpt")

# Lists of flags for various build types
if (CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
  if (CMAKE_SYSTEM_PROCESSOR MATCHES arm64)
    set(RELEASE_FLAGS "-O3 -mtune=native -mcpu=apple-m1 -DNDEBUG")
  else()
    set(RELEASE_FLAGS "-O3 -mtune=native -march=native -DNDEBUG")
  endif()
else()
    set(RELEASE_FLAGS "-O3 -mtune=native -march=native -DNDEBUG")
endif()
#set(RELEASE_FLAGS "-O3 -mtune=native -march=native -DNDEBUG")
#set(RELEASE_FLAGS "-O3 -mtune=native -march=native")
set(DEBUG_FLAG "-g")
set(WARN_FLAGS_CLANG "-Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic")
#set(WARN_FLAGS_GCC "-Wall -Wextra -Wpedantic -Weffc++ -Wno-c++98-compat -Wno-c++98-compat-pedantic")
set(WARN_FLAGS_GCC "-Wall -Wextra -Wpedantic -Wno-c++98-compat -Wno-c++98-compat-pedantic")

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    set(WARN_FLAGS ${WARN_FLAGS_CLANG})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    # using Clang
    set(WARN_FLAGS ${WARN_FLAGS_CLANG})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using gcc
    set(WARN_FLAGS ${WARN_FLAGS_GCC})
else()
    # Unsupported compiler
    # If you are are windows, whelp... I got 99 problems but M$ ain't one of them
    # If you are using the Intel compiler, well... that decision is between you and your deity
    set(WARN_FLAGS "")
    message(SEND_ERROR "Compilers other than `clang` and `gcc` are not supported.")
endif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

###########################################################
##  Setup Flag To Tell Code The Compiler (Simplifying Checks)
###########################################################
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # using Clang
    set(COMPILER_NAME_FLAG "-DCLANG_COMPILER=1")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    # using Clang
    set(COMPILER_NAME_FLAG "-DCLANG_COMPILER=1")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(COMPILER_NAME_FLAG "-DGCC_COMPILER=1")
else()
    # Unsupported compiler
endif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

# Handle if the user set things to use a sanitizer
if(SANITIZER STREQUAL "Address")
    set(SANITIZER_CXX_FLAGS  "-fsanitize=address -DBOOST_USE_ASAN=1")
    set(SANITIZER_LINKER_FLAGS  "-fsanitize=address")
elseif(SANITIZER STREQUAL "Thread")
    set(SANITIZER_CXX_FLAGS  "-fsanitize=thread")
    set(SANITIZER_LINKER_FLAGS  "-fsanitize=thread")
elseif(SANITIZER STREQUAL "Memory")
    set(SANITIZER_CXX_FLAGS  "-fsanitize=memory")
    set(SANITIZER_LINKER_FLAGS  "-fsanitize=memory")
elseif(SANITIZER STREQUAL "UndefinedBehavior")
    set(SANITIZER_CXX_FLAGS  "-fsanitize=undefined")
    set(SANITIZER_LINKER_FLAGS  "-fsanitize=undefined")
elseif(SANITIZER STREQUAL "Leak")
    set(SANITIZER_CXX_FLAGS  "-fsanitize=leak")
    set(SANITIZER_LINKER_FLAGS  "-fsanitize=leak")
endif(SANITIZER STREQUAL "Address")

# set up for the plain build type
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${COMPILER_NAME_FLAG} ${SANITIZER_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZER_LINKER_FLAGS}")
# set up debug flags
set(CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS} ${COMPILER_NAME_FLAG} ${DEBUG_FLAG} ${SANITIZER_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG  "${CMAKE_EXE_LINKER_FLAGS} ${DEBUG_FLAG} ${SANITIZER_LINKER_FLAGS}")
# set up release flags
set(CMAKE_CXX_FLAGS_RELEASE  "${CMAKE_CXX_FLAGS} ${COMPILER_NAME_FLAG} ${RELEASE_FLAGS} ${SANITIZER_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE  "${CMAKE_EXE_LINKER_FLAGS} ${RELEASE_FLAGS} ${SANITIZER_LINKER_FLAGS}")
# set up release with debug flags
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS} ${COMPILER_NAME_FLAG} ${DEBUG_FLAG} ${RELEASE_FLAGS} ${SANITIZER_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO  "${CMAKE_EXE_LINKER_FLAGS} ${DEBUG_FLAG} ${RELEASE_FLAGS} ${SANITIZER_LINKER_FLAGS}")
# set up warn flags
set(CMAKE_CXX_FLAGS_WARN  "${CMAKE_CXX_FLAGS} ${COMPILER_NAME_FLAG} ${WARN_FLAGS} ${SANITIZER_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_WARN  "${CMAKE_EXE_LINKER_FLAGS} ${WARN_FLAGS} ${SANITIZER_LINKER_FLAGS}")
# set up warn with optimization flags
set(CMAKE_CXX_FLAGS_WARNWITHOPT  "${CMAKE_CXX_FLAGS} ${COMPILER_NAME_FLAG} ${RELEASE_FLAGS} ${WARN_FLAGS} ${SANITIZER_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_WARNWITHOPT  "${CMAKE_EXE_LINKER_FLAGS} ${RELEASE_FLAGS} ${WARN_FLAGS} ${SANITIZER_LINKER_FLAGS}")
