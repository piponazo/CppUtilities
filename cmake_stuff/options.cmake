#------------------------------------------------------
# Build type
#------------------------------------------------------
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
if(DEFINED CMAKE_BUILD_TYPE)
   SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
endif()

if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Debug")
endif()

# ----------------------------------------------------------------------------
#   PROJECT CONFIGURATION
# ----------------------------------------------------------------------------
option(USE_CLANG            "Use Clang compiler"                                            OFF)
option(WARNINGS_ARE_ERRORS  "Treat warnings as errors"                                      ON)
option(WARNINGS_ANSI_ISO    "Issue all the mandatory diagnostics Listed in C standard"      ON)
option(ENABLE_COVERAGE      "Perform code coverage in HTML"                                 OFF)
option(ENABLE_COVERAGE_LCOV "Perform code coverage in HTML"                                 OFF)
option(ENABLE_COVERAGE_GCOVR "Perform code coverage in XML for jenkins integration"          OFF)

option(INSTALL_DOC          "Install documentation in system"                               OFF)
option(DOXY_COVERAGE        "Generate text file with the doxygen coverage"                  OFF)
option(USE_MATHJAX          "Generate doc-formulas via mathjax instead of latex"            OFF)
option(USE_DOT              "Diagram generation with graphviz"                              ON)
option(USE_LATEX            "Build latex documentation"                                     OFF)
option(USE_CHM              "Build CHM Windows documentation"                               OFF)

option(BUILD_SHARED_LIBS    "Build shared libraries"                                        ON)
option(BUILD_TESTS          "Build tests (unitary, integration)"                            OFF)

if(ENABLE_COVERAGE_LCOV)
    set(ENABLE_COVERAGE ON)
    include(cmake_stuff/code_coverage.cmake REQUIRED)
endif()

if(ENABLE_COVERAGE_GCOVR)
    set(ENABLE_COVERAGE ON)
    include(cmake_stuff/code_coverage_xml.cmake)
endif()

if((ENABLE_COVERAGE OR ENABLE_COVERAGE_XML) AND NOT BUILD_TESTS)
   message(WARNING "It's necessary to build unitary tests for checking code coverage")
endif()

if(ENABLE_COVERAGE OR ENABLE_COVERAGE_XML)
   add_custom_target(cleanCoverage
      COMMAND find -name *.gcda -delete
      COMMAND find -name *.gcno -delete
      COMMAND rm -rf coverage*
      COMMAND rm -f *info*
      COMMAND make clean)
endif()

set(CMAKE_INCLUDE_DIRS_CONFIGCMAKE ${CMAKE_INSTALL_PREFIX}/include      CACHE PATH "Output directory for headers")
set(CMAKE_LIB_DIRS_CONFIGCMAKE     ${CMAKE_INSTALL_PREFIX}/lib          CACHE PATH "Output directory for libraries")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

include (GenerateExportHeader)
