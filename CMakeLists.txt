project(CppUtilities)
cmake_minimum_required(VERSION 2.8)

include(cmake_stuff/versions.cmake              REQUIRED)
include(cmake_stuff/options.cmake               REQUIRED)
include(cmake_stuff/findDependencies.cmake      REQUIRED)
include(cmake_stuff/compileroptions.cmake       REQUIRED)
include(cmake_stuff/generateDoc.cmake           REQUIRED)
include(cmake_stuff/buildFiles.cmake            REQUIRED)

GENERATE_DOCUMENTATION(${PROJECT_SOURCE_DIR}/cmake_stuff/dox.in)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(apps)

include(cmake_stuff/packageGenerator.cmake      REQUIRED)
include(cmake_stuff/printInfo.cmake             REQUIRED)
