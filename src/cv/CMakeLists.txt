include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE core_hdrs_inc "${PROJECT_SOURCE_DIR}/include/minicv/*.h"
                           "${PROJECT_SOURCE_DIR}/include/minicv/*.hpp")
file(GLOB_RECURSE core_hdrs "*.h" "*.hpp")
file(GLOB_RECURSE core_srcs "*.cpp")

add_library(${PROJECT_NAME}_cvCore STATIC ${core_srcs} ${core_hdrs} ${core_hdrs_inc})
