include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_BINARY_DIR})

file(GLOB_RECURSE hdrs_inc "${PROJECT_SOURCE_DIR}/include/${LIB}/*.h")
file(GLOB_RECURSE hdrs "*.h")
file(GLOB_RECURSE srcs "*.cpp")

add_library(${PROJECT_NAME} STATIC ${srcs} ${hdrs} ${hdrs_inc})

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${VERSION}
    SOVERSION ${SOVERSION}
    CLEAN_DIRECT_OUTPUT 1
)

generate_export_header(${PROJECT_NAME}
    EXPORT_MACRO_NAME EXPORT
    EXPORT_FILE_NAME "${PROJECT_BINARY_DIR}/${PROJECT_NAME}_export.h"
)

install(TARGETS ${PROJECT_NAME}${LIB}
    RUNTIME DESTINATION bin COMPONENT libraries
    LIBRARY DESTINATION lib COMPONENT libraries
    ARCHIVE DESTINATION lib COMPONENT libraries
)

set(hdrs_inc ${hdrs_inc} ${PROJECT_NAME}_export.h)

install(FILES ${hdrs_inc}
    DESTINATION include/${PROJECT_NAME}/
    COMPONENT headers
)
