configure_file("${PROJECT_SOURCE_DIR}/cmake_stuff/cmake_uninstall.cmake.in" "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${PROJECT_BINARY_DIR}/cmake_uninstall.cmake")

configure_file("${PROJECT_SOURCE_DIR}/cmake_stuff/config.h.in" "${PROJECT_BINARY_DIR}/config.h" @ONLY)
configure_file("${PROJECT_SOURCE_DIR}/cmake_stuff/config.cmake.in" "${PROJECT_BINARY_DIR}/Find${PROJECT_NAME}.cmake" @ONLY)
install(FILES "${PROJECT_BINARY_DIR}/Find${PROJECT_NAME}.cmake"
   DESTINATION lib/cmake/
   COMPONENT headers
   )

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
