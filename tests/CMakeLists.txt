if(NOT GTEST_FOUND)

   message(WARNING "GTest is not installed in your system so tests can't be compiled")

elseif(BUILD_TESTS)

    include_directories(${PROJECT_BINARY_DIR}/)
    include_directories(${PROJECT_SOURCE_DIR}/include)
    include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})

    add_subdirectory(libForExtensibleFactory)
    add_subdirectory(unitary)

    # Generate a single application including all the tests
    add_executable(test_all
        test_all.cpp
        ${srcs_unit_all}
    )

    target_link_libraries(test_all
        ${GTEST_BOTH_LIBRARIES}
        ${PROJECT_NAME}
        ${PROJECT_NAME}_cvCore pthread
                ExtTest
    )

    if(ENABLE_COVERAGE_LCOV)
        SETUP_TARGET_FOR_COVERAGE(coverageHtml test_all coverage)
    endif()

    if(ENABLE_COVERAGE_GCOVR)
        SETUP_TARGET_FOR_COVERAGE_XML(coverageXml test_all coverage)
    endif()

endif()
