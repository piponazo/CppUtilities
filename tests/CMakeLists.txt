add_subdirectory(libForExtensibleFactory)
add_subdirectory(unitary)

# Generate a single application including all the tests
add_executable(test_all
    test_all.cpp
    ${srcs_unit_all}
)

target_link_libraries(test_all
    utilities
    ExtTest
    Catch2::Catch2
)

target_include_directories(test_all
    PRIVATE
        ${PROJECT_BINARY_DIR}
)
