add_library(ExtTest 
    implementation1.cpp
    implementation1.h
    implementation2.cpp
    implementation2.h
    interface.cpp
    interface.h
)

generate_export_header(ExtTest)

target_include_directories(ExtTest PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

target_link_libraries(ExtTest PRIVATE utilities)
