###########################################################
##  Setup clang tidy for lots of warnings and src analysis
###########################################################
set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")

find_program(
  CLANG_TIDY_EXE NAMES clang-tidy clang-tidy-12 clang-tidy-13 clang-tidy-14
  DOC "Path to clang-tidy executable")

find_program(
  RUN_CLANG_TIDY_PYTHON "${CMAKE_CURRENT_SOURCE_DIR}/tooling/run-clang-tidy.py"
  DOC "Path to python script to run clang-tidy on build database")

list(APPEND CLANG_TIDY_CHECKS_LIST
     "abseil-*,"
     "-abseil-string-find-str-contains," # most abseil warnings have been useful even without the lib, not this one though
     # "altera-*," # not writing for an FPGA here so only here for posterity
     "android-*,"
     "boost-*,"
     "bugprone-*,"
     "cert-*,"
     "-cert-err58-cpp," # if there is a thrown exception while initting static storage members, I want a throw
     "clang-*,"
     "concurency-*,"
     "cppcoreguidelines-*,"
     "-cppcoreguidelines-macro-usage," # nolint is not killing complaints about my warning silencing macros, so screw it
     "-cppcoreguidelines-avoid-c-arrays," # c-style arrays are too useful for purposes of having a pointer
     "-cppcoreguidelines-owning-memory," # I am NOT using the core guidelines gsl
     "-cppcoreguidelines-pro-bounds-pointer-arithmetic," # pointers are numbers and sometimes you have to use them as such
     "-cppcoreguidelines-avoid-magic-numbers," # 5 is not a flipping magic number when it is a static index into an array
     "-cppcoreguidelines-pro-bounds-constant-array-index," # OMG indexing with an expression is fine wtf
     "-cppcoreguidelines-no-malloc," # if I want aligned memory I want malloc so screw this"
     "-cppcoreguidelines-non-private-member-variables-in-classes," # making classes have all private members is ridiculous, if a member is accessed trivially, screw getters and setters
     "-cppcoreguidelines-pro-bounds-array-to-pointer-decay," # ARRAYS __ARE__ POINTERS!!!!!!!!!!!!!!!!!!!!!!
     "-cppcoreguidelines-pro-type-reinterpret-cast," # avoid the hell out of it, but sometimes you _have_ to
     "darwin-*,"
     "fuchsia-*"
     "google-*,"
     "hicpp-*,"
     "-hicpp-avoid-c-arrays," # I see no simple way to force alignment of Non-C arrays, so screw this
     "-hicpp-no-array-decay," # look, arrays _are_ pointers, I will decay them if I want to
     "-hicpp-no-malloc," # sometimes you _have_ to manage memory manually, especially with aligned allocation
     "linuxkernel-*,"
     "llvm-*,"
     "-llvm-include-order," # I have my own include ordering that I prefer
     "-llvm-header-guard," # I have my own header guard style that I prefer
     "misc-*,"
     "-misc-non-private-member-variables-in-classes," # I will not be screamed at for having structs for mass freight of data, but they have a "reset" member function
     "modernize-*,"
     "-modernize-avoid-c-arrays,"  # I see no simple way to force alignment of Non-C arrays, so screw this
     "-modernize-use-trailing-return-type," # I personally dislike this style because I visually search by return type, then by function name
     # "mpi-*," # not using MPI here so this is only in the list for posterity that the options are available
     "performance-*,"
     "portability-*,"
     "zircon-*")

list(JOIN CLANG_TIDY_CHECKS_LIST "" CLANG_TIDY_CHECKS)

list(APPEND RUN_CLANG_TIDY_BIN_ARGS
         -clang-tidy-binary ${CLANG_TIDY_EXE}
         -header-filter="${CMAKE_CURRENT_SOURCE_DIR}"
         -checks=${CLANG_TIDY_CHECKS})

add_custom_target(tidy
                  COMMAND ${RUN_CLANG_TIDY_PYTHON} ${RUN_CLANG_TIDY_BIN_ARGS}
                  COMMENT "Running `clang-tidy`")
