add_library(utilities
    include/utilities/ExtensibleFactory.h
    include/utilities/histogram1D.h
    include/utilities/macros.h
    include/utilities/matrix.h

    histogram1D.cpp
    matrix.cpp
)

generate_export_header(utilities)

target_include_directories(utilities PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

target_include_directories(utilities PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


install(TARGETS utilities
    RUNTIME DESTINATION bin COMPONENT libraries
    LIBRARY DESTINATION lib COMPONENT libraries
    ARCHIVE DESTINATION lib COMPONENT libraries
)

#set(hdrs_inc ${hdrs_inc} ${PROJECT_NAME}_export.h)

install(FILES ${hdrs_inc}
    DESTINATION include/${PROJECT_NAME}/
    COMPONENT headers
)
