cmake_minimum_required(VERSION 3.13)

project(CppUtilities
    DESCRIPTION "C++ Utilities and experiments with modern C++"
    VERSION 0.0.1
    LANGUAGES CXX
)

include(cmake_stuff/versions.cmake              REQUIRED)
include(cmake_stuff/options.cmake               REQUIRED)
include(cmake_stuff/findDependencies.cmake      REQUIRED)
include(cmake_stuff/compileroptions.cmake       REQUIRED)
include(cmake_stuff/generateDoc.cmake           REQUIRED)
include(cmake_stuff/buildFiles.cmake            REQUIRED)

GENERATE_DOCUMENTATION(${PROJECT_SOURCE_DIR}/cmake_stuff/dox.in)

add_subdirectory(src)
add_subdirectory(apps)

if (Catch2_FOUND)
    add_subdirectory(tests)
endif()

include(cmake_stuff/packageGenerator.cmake      REQUIRED)
include(cmake_stuff/printInfo.cmake             REQUIRED)
