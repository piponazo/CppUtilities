include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR})

file(GLOB_RECURSE hdrs "*.h")
file(GLOB_RECURSE srcs "*.cpp")

add_library(ExtTest ${srcs} ${hdrs})

set_target_properties(ExtTest PROPERTIES
    VERSION ${VERSION}
    SOVERSION ${SOVERSION}
    CLEAN_DIRECT_OUTPUT 1
)

generate_export_header(ExtTest
    EXPORT_MACRO_NAME EXPORT
	EXPORT_FILE_NAME "${PROJECT_BINARY_DIR}/ExtTest_export.h"
)
