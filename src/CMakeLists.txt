add_library(utilities
    include/utilities/ExtensibleFactory.h
    include/utilities/histogram1D.h
    include/utilities/macros.h
    include/utilities/matrix.h
    include/utilities/sizeLiterals.h
    include/utilities/FactorySFINAE.h

    histogram1D.cpp
    matrix.cpp
)

generate_export_header(utilities)

# Make available the current directory where the export.h header is
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

install(FILES ${hdrs_inc}
    DESTINATION include/${PROJECT_NAME}/
    COMPONENT headers
)
