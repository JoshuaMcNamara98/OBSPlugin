## CMake common compiler options module

include_guard(GLOBAL)

# Set C and C++ language standards to C17 and C++17
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Set symbols to be hidden by default for C and C++
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN TRUE)

# clang options for C, C++, ObjC, and ObjC++
set(
  _obs_clang_common_options
  -fno-strict-aliasing
  -Wno-trigraphs
  -Wno-missing-field-initializers
  -Wno-missing-prototypes
  -Werror=return-type
  -Wunreachable-code
  -Wquoted-include-in-framework-header
  -Wno-missing-braces
  -Wparentheses
  -Wswitch
  -Wno-unused-function
  -Wno-unused-label
  -Wunused-parameter
  -Wunused-variable
  -Wunused-value
  -Wempty-body
  -Wuninitialized
  -Wno-unknown-pragmas
  -Wfour-char-constants
  -Wconstant-conversion
  -Wno-conversion
  -Wint-conversion
  -Wbool-conversion
  -Wenum-conversion
  -Wnon-literal-null-conversion
  -Wsign-compare
  -Wshorten-64-to-32
  -Wpointer-sign
  -Wnewline-eof
  -Wno-implicit-fallthrough
  -Wdeprecated-declarations
  -Wno-sign-conversion
  -Winfinite-recursion
  -Wcomma
  -Wno-strict-prototypes
  -Wno-semicolon-before-method-body
  -Wformat-security
  -Wvla
  -Wno-error=shorten-64-to-32
)

# clang options for C
    -Wunused-parameter
    -Wunused-value
    -Wunused-variable
    -Wvla)

# clang options for C++
set(_obs_clang_cxx_options
    # cmake-format: sortable
    ${_obs_clang_c_options}
    -Wconversion
    -Wdeprecated-implementations
    -Wduplicate-method-match
    -Wfloat-conversion
    -Wfour-char-constants
    -Wimplicit-retain-self
    -Winvalid-offsetof
    -Wmove
    -Wno-c++11-extensions
    -Wno-exit-time-destructors
    -Wno-implicit-atomic-properties
    -Wno-objc-interface-ivars
    -Wno-overloaded-virtual
    -Wrange-loop-analysis)

if(NOT DEFINED CMAKE_COMPILE_WARNING_AS_ERROR)
  set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
endif()
