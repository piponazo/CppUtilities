include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR})

file(GLOB_RECURSE hdrs "*.h")
file(GLOB_RECURSE srcs "*.cpp")

add_library(ExtTest ${srcs} ${hdrs})

generate_export_header(ExtTest
    EXPORT_FILE_NAME "${PROJECT_BINARY_DIR}/ExtTest_export.h"
)

target_link_libraries(ExtTest PRIVATE utilities)