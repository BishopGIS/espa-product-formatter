
include(GNUInstallDirs)
include(CheckCCompilerFlag)
include(CheckCSourceCompiles)
include(CheckIncludeFiles)
include(CheckIncludeFile)
include(CheckTypeSize)
include(CheckFunctionExists)
include(TestBigEndian)

# Check if the compiler supports each of the following additional
# flags, and enable them if supported.  This greatly improves the
# quality of the build by checking for a number of common problems,
# some of which are quite serious.
if(CMAKE_C_COMPILER_ID STREQUAL "GNU" OR
   CMAKE_C_COMPILER_ID MATCHES "Clang")
  set(test_flags
      -Wall
      -Winline
      -W
      -Wformat-security
      -Wpointer-arith
      -Wdisabled-optimization
      -Wno-unknown-pragmas
      -Wdeclaration-after-statement
      -fstrict-aliasing)
  if(extra-warnings)
    list(APPEND test_flags
        -Wfloat-equal
        -Wmissing-prototypes
        -Wunreachable-code)
  endif()
  if(fatal-warnings)
    list(APPEND test_flags
         -Werror)
  endif()
elseif(MSVC)
  set(test_flags)
  if(extra-warnings)
    list(APPEND test_flags
         /W4)
  else()
    list(APPEND test_flags
         /W3)
  endif()
  if (fatal-warnings)
    list(APPEND test_flags
         /WX)
  endif()
endif()

foreach(flag ${test_flags})
  string(REGEX REPLACE "[^A-Za-z0-9]" "_" flag_var "${flag}")
  set(test_c_flag "C_FLAG${flag_var}")
  CHECK_C_COMPILER_FLAG(${flag} "${test_c_flag}")
  if (${test_c_flag})
     set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${flag}")
  endif (${test_c_flag})
endforeach(flag ${test_flags})

if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

option(ld-version-script "Enable linker version script" ON)
# Check if LD supports linker scripts.
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/conftest.map" "VERS_1 {
        global: sym;
};

VERS_2 {
        global: sym;
} VERS_1;
")
set(CMAKE_REQUIRED_FLAGS_SAVE ${CMAKE_REQUIRED_FLAGS})
set(CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS} "-Wl,--version-script=${CMAKE_CURRENT_BINARY_DIR}/conftest.map")
check_c_source_compiles("int main(void){return 0;}" HAVE_LD_VERSION_SCRIPT)
set(CMAKE_REQUIRED_FLAGS ${CMAKE_REQUIRED_FLAGS_SAVE})
file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/conftest.map")
if (ld-version-script AND HAVE_LD_VERSION_SCRIPT)
  set(HAVE_LD_VERSION_SCRIPT TRUE)
else()
  set(HAVE_LD_VERSION_SCRIPT FALSE)
endif()

# Find libm, if available
find_library(M_LIBRARY m)

check_include_file(assert.h    HAVE_ASSERT_H)
check_include_file(dlfcn.h     HAVE_DLFCN_H)
check_include_file(fcntl.h     HAVE_FCNTL_H)
check_include_file(inttypes.h  HAVE_INTTYPES_H)
check_include_file(io.h        HAVE_IO_H)
check_include_file(stdio.h     HAVE_STDIO_H)
check_include_file(limits.h    HAVE_LIMITS_H)
check_include_file(malloc.h    HAVE_MALLOC_H)
check_include_file(memory.h    HAVE_MEMORY_H)
check_include_file(search.h    HAVE_SEARCH_H)
check_include_file(stdint.h    HAVE_STDINT_H)
check_include_file(stdlib.h    HAVE_STDLIB_H)
check_include_file(stdarg.h    HAVE_STDARG_H)
check_include_file(string.h    HAVE_STRING_H)
check_include_file(strings.h   HAVE_STRINGS_H)
check_include_file(sys/time.h  HAVE_SYS_TIME_H)
check_include_file(time.h      HAVE_TIME_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(unistd.h    HAVE_UNISTD_H)
check_include_file(getopt.h    HAVE_GETOPT_H)
check_include_file(ctype.h     HAVE_CTYPE_H)
check_include_file(math.h      HAVE_MATH_H)
check_include_file(wchar.h     HAVE_WCHAR_H)
check_include_file(stddef.h    HAVE_STDDEF_H)


check_function_exists (flockfile  HAVE_FLOCKFILE)
check_function_exists (funlockfile HAVE_FUNLOCKFILE)

# Inspired from /usr/share/autoconf/autoconf/c.m4
foreach(inline_keyword "inline" "__inline__" "__inline")
  if(NOT DEFINED C_INLINE)
    set(CMAKE_REQUIRED_DEFINITIONS_SAVE ${CMAKE_REQUIRED_DEFINITIONS})
    set(CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS}
        "-Dinline=${inline_keyword}")
    check_c_source_compiles("
        typedef int foo_t;
        static inline foo_t static_foo() {return 0;}
        foo_t foo(){return 0;}
        int main(int argc, char *argv[]) {return 0;}"
      C_HAS_${inline_keyword})
    set(CMAKE_REQUIRED_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS_SAVE})
    if(C_HAS_${inline_keyword})
      set(C_INLINE TRUE)
      set(INLINE_KEYWORD "${inline_keyword}")
    endif()
 endif()
endforeach()
if(NOT DEFINED C_INLINE)
  set(INLINE_KEYWORD)
endif()

check_function_exists(getopt      HAVE_GETOPT)
check_function_exists(getopt_long HAVE_GETOPT_LONG)

check_function_exists(snprintf    HAVE_SNPRINTF)
check_function_exists(sprintf     HAVE_SPRINTF)
check_function_exists(vfprintf    HAVE_VFPRINTF)
check_function_exists(vsnprintf   HAVE_VSNPRINTF)
check_function_exists(vsprintf    HAVE_VSPRINTF)

if(NOT HAVE_SNPRINTF)
  add_definitions(-DNEED_LIBPORT)
endif()

# CPU bit order
set(fillorder FILLORDER_MSB2LSB)
if(CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "i.*86.*" OR
   CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "amd64.*" OR
   CMAKE_HOST_SYSTEM_PROCESSOR MATCHES "x86_64.*")
  set(fillorder FILLORDER_LSB2MSB)
endif()
set(HOST_FILLORDER ${fillorder} CACHE STRING "Native CPU bit order")
mark_as_advanced(HOST_FILLORDER)

# CPU endianness
test_big_endian(WORDS_BIGENDIAN)
if (WORDS_BIGENDIAN)
  set(WORDS_BIGENDIAN 1)
else()
  set(HAVE_LITTLE_ENDIAN 1)
endif()

# IEEE floating point
set(HAVE_IEEEFP 1 CACHE STRING "IEEE floating point is available")
mark_as_advanced(HAVE_IEEEFP)

check_type_size(uintptr_t UINTPTR_T)
if(NOT HAVE_UINTPTR_T)
  if("${CMAKE_SIZEOF_VOID_P}" EQUAL 8)
    set(uintptr_t "uint64_t")
  else()
    set(uintptr_t "uint32_t")
  endif()
endif()    
   
check_include_files("unistd.h" HAVE_UNISTD_H)
    
check_function_exists("utime" HAVE_UTIME)    
check_function_exists("utimes" HAVE_UTIMES)
    
check_c_source_compiles("
    extern __attribute__((__visibility__(\"hidden\"))) int hiddenvar;
    extern __attribute__((__visibility__(\"default\"))) int exportedvar;
    extern __attribute__((__visibility__(\"hidden\"))) int hiddenfunc (void);
    extern __attribute__((__visibility__(\"default\"))) int exportedfunc (void);
    void dummyfunc (void) {}
    int main (){
      return 0;
    }    
    " HAVE_VISIBILITY)
    
check_type_size(pid_t PID_T)  
check_type_size(size_t SIZEOF_SIZE_T)    
check_type_size(int16_t INT16_T)
check_type_size(int32_t INT32_T)
check_type_size(int64_t INT64_T)
check_type_size(intmax_t INTMAX_T)
check_type_size(uint8_t UINT8_T)
check_type_size(uint16_t UINT16_T)
check_type_size(uint32_t UINT32_T)
check_type_size(uint64_t UINT64_T)
check_type_size(uintmax_t UINTMAX_T)

check_type_size("short" SIZE_OF_SHORT)
check_type_size("int" SIZE_OF_INT)
check_type_size("long" SIZE_OF_LONG)
check_type_size("long long" SIZE_OF_LONG_LONG)

check_type_size("unsigned short" SIZE_OF_UNSIGNED_SHORT)
check_type_size("unsigned" SIZE_OF_UNSIGNED)
check_type_size("unsigned long" SIZE_OF_UNSIGNED_LONG)
check_type_size("unsigned long long" SIZE_OF_UNSIGNED_LONG_LONG)
check_type_size("size_t" SIZE_OF_SIZE_T)

check_type_size("__int64" __INT64)
check_type_size("unsigned __int64" UNSIGNED___INT64)
        
configure_file(${CMAKE_MODULE_PATH}/config.h.cmakein ${CMAKE_CURRENT_BINARY_DIR}/config.h IMMEDIATE @ONLY)
add_definitions (-DHAVE_CONFIG_H)