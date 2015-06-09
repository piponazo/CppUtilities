include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE core_hdrs_inc "${PROJECT_SOURCE_DIR}/include/cv/core/*.h"
                           "${PROJECT_SOURCE_DIR}/include/cv/core/*.hpp")
file(GLOB_RECURSE core_hdrs "core/*.h" "core/*.hpp")
file(GLOB_RECURSE core_srcs "core/*.cpp")

file(GLOB_RECURSE hal_hdrs_inc "${PROJECT_SOURCE_DIR}/include/cv/hal/*.h"
                           "${PROJECT_SOURCE_DIR}/include/cv/hal/*.hpp")
file(GLOB_RECURSE hal_hdrs "hal/*.h" "hal/*.hpp")
file(GLOB_RECURSE hal_srcs "hal/*.cpp")

add_library(${PROJECT_NAME}_cvHal STATIC ${hal_srcs} ${hal_hdrs} ${hal_hdrs_inc})
add_library(${PROJECT_NAME}_cvCore STATIC ${core_srcs} ${core_hdrs} ${core_hdrs_inc})

target_link_libraries(${PROJECT_NAME}_cvCore ${PROJECT_NAME}_cvHal)
