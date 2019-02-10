set(EXTRA_C_FLAGS "")
set(EXTRA_C_FLAGS_RELEASE "")
set(EXTRA_C_FLAGS_DEBUG "")
set(EXTRA_EXE_LINKER_FLAGS "")
set(EXTRA_EXE_LINKER_FLAGS_RELEASE "")
set(EXTRA_EXE_LINKER_FLAGS_DEBUG "")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    set(COMPILER_IS_GCC ON)
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(COMPILER_IS_CLANG ON)
endif()


if (UNIX)
    set (CMAKE_CXX_FLAGS                "")
    set (CMAKE_CXX_FLAGS_DEBUG          "-g -O0 -DDEBUG")
    set (CMAKE_CXX_FLAGS_RELEASE        "-O2 -DNDEBUG")
elseif(WIN32)
   if(MSVC)
      string(REGEX REPLACE "^  *| * $" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
      string(REGEX REPLACE "^  *| * $" "" CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT}")
      if(CMAKE_CXX_FLAGS STREQUAL CMAKE_CXX_FLAGS_INIT)
  
         # override cmake default exception handling option
         string(REPLACE "/EHsc" "/EHa" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}"  CACHE STRING "Flags used by the compiler during all build types." FORCE)
      endif()
   endif()
endif()

if (COMPILER_IS_GCC OR COMPILER_IS_CLANG)
   if(WARNINGS_ANSI_ISO)
      set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -pedantic -Wextra")
   endif()

   if(WARNINGS_ARE_ERRORS)
      set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Werror")
   endif()

   if(WARNINGS_EFFCPP)
      set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Weffc++")
   endif()

   set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wall -Werror=return-type")

endif()

if (COMPILER_IS_GCC)
    set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fvisibility=hidden") # All the symbols will be hidden by default.
    set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wreturn-type")
    set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wno-long-long")
    set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wno-maybe-uninitialized")

    if(WARNINGS_ANSI_ISO)
        set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wstrict-aliasing=3")
    else()
        set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wno-narrowing")
        set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wno-delete-non-virtual-dtor")
        set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wno-unnamed-type-template-args")
    endif()

    if(ENABLE_COVERAGE)
        set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} --coverage")
    endif()

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)

#    if(NOT EXISTS ${CMAKE_CXX_COMPILER})
#        message( FATAL_ERROR "Clang++ not found. " )
#    endif()

    if(ENABLE_COVERAGE OR ENABLE_COVERAGE_XML)
        message(FATAL_ERROR "Coverage feature only supported by GCC.")
    endif()

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)

   set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /wd4503 /wd4985 /wd4291 /W3")
   set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /MP /Qpar")
   set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /Gy") # Remove unreferenced functions: function level linking
#   set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /openmp")

#   set(EXTRA_C_FLAGS_RELEASE "/MD /O2 /Ob2 /DNDEBUG") # Overriding default release flags
#   set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /Zi")# Produces PDBs & it implies /DEBUG.
#   set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /Oy-") # This is only available in x86
# Disables the Frame-pointer omission. /Ox, /O1 & O2 implies /Oy. /Oy- disbles it. It improves the
# debugging experience

#set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /Gm")# For minimal rebuilds (incompatible with /MP)
   set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE} /arch:AVX2")

   set(EXTRA_C_FLAGS_DEBUG "${EXTRA_C_FLAGS_DEBUG} /DEBUG")

   set(EXTRA_EXE_LINKER_FLAGS "${EXTRA_EXE_LINKER_FLAGS} /ignore:4049 /ignore:4217 /ignore:4099 /INCREMENTAL:NO")
   set(EXTRA_EXE_LINKER_FLAGS_RELEASE "${EXTRA_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF /NODEFAULTLIB:libcmt")
   set(EXTRA_EXE_LINKER_FLAGS_DEBUG "${EXTRA_EXE_LINKER_FLAGS_DEBUG} /OPT:NOREF /OPT:NOICF /NODEFAULTLIB:MSVCRT")

   if(NOT MSVC_VERSION LESS 1400) # From VS 9.0 in advance
          set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /bigobj")
   endif()

   if(WARNINGS_ARE_ERRORS)
          set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /WX")
   endif()

endif()

#include(cmake_stuff/dynamic_analyzer_options.cmake    REQUIRED)

# Add user supplied extra options (optimization, etc...)
# ==========================================================
set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS}" CACHE INTERNAL "Extra compiler options")
set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE}" CACHE INTERNAL "Extra compiler options for Release build")
set(EXTRA_C_FLAGS_DEBUG "${EXTRA_C_FLAGS_DEBUG}" CACHE INTERNAL "Extra compiler options for Debug build")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_C_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}  ${EXTRA_C_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${EXTRA_C_FLAGS_DEBUG}")

set(CMAKE_EXE_LINKER_FLAGS         "${CMAKE_EXE_LINKER_FLAGS} ${EXTRA_EXE_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${EXTRA_EXE_LINKER_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG   "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${EXTRA_EXE_LINKER_FLAGS_DEBUG}")
