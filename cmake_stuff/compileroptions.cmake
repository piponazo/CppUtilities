set(EXTRA_C_FLAGS "")
set(EXTRA_C_FLAGS_RELEASE "")
set(EXTRA_C_FLAGS_DEBUG "")
set(EXTRA_EXE_LINKER_FLAGS "")
set(EXTRA_EXE_LINKER_FLAGS_RELEASE "")
set(EXTRA_EXE_LINKER_FLAGS_DEBUG "")

if (UNIX)
    if (USE_CLANG)
        set (CMAKE_C_COMPILER_ID            "Clang")
        set (CMAKE_CXX_COMPILER_ID          "Clang")
        set (CMAKE_C_COMPILER               "/usr/bin/clang")
        set (CMAKE_CXX_COMPILER             "/usr/bin/clang++")

        set (CMAKE_CXX_FLAGS                "")
        set (CMAKE_CXX_FLAGS_DEBUG          "-g -O0 -DDEBUG")
        set (CMAKE_CXX_FLAGS_RELEASE        "-O2 -DNDEBUG")
    else()
        set (CMAKE_CXX_FLAGS                "")
        set (CMAKE_CXX_FLAGS_DEBUG          "-g -O0 -DDEBUG")
#        set (CMAKE_CXX_FLAGS_RELEASE        "-O2")
    endif()
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR
    ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")

   if(WARNINGS_ANSI_ISO)
      set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -pedantic -Wextra")
   endif()

   if(WARNINGS_ARE_ERRORS)
      set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Werror")
   endif()

   if(WARNINGS_EFFCPP)
      set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Weffc++")
   endif()

   set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -std=c++11")
   set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wall -Werror=return-type")

endif()

IF (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
   execute_process(
   COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
   if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
      message(FATAL_ERROR "${PROJECT_NAME} c++11 support requires g++ 4.7 or greater.")
   endif ()

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

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

#    if(NOT EXISTS ${CMAKE_CXX_COMPILER})
#        message( FATAL_ERROR "Clang++ not found. " )
#    endif()

    if(ENABLE_COVERAGE OR ENABLE_COVERAGE_XML)
        message(FATAL_ERROR "Coverage feature only supported by GCC.")
    endif()

else ()

    message( FATAL_ERROR "Not supported compiler: ${CMAKE_CXX_COMPILER_ID}. " )

endif()

include(cmake_stuff/dynamic_analyzer_options.cmake    REQUIRED)

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
