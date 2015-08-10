if(NOT CPACK_GENERATOR)
    if (UNIX)
        set(CPACK_GENERATOR "DEB" )
    elseif (WIN32)
       message(WARNING "CPack not supported yet in windows")
    endif()
endif()

include(InstallRequiredSystemLibraries)
set(CPACK_PACKAGE_MAINTAINER          "Luis Díaz Más (piponazo@gmail.com)")
set(CPACK_PACKAGE_CONTACT             "Luis Díaz Más (piponazo@gmail.com)")
set(CPACK_PACKAGE_NAME                "${PROJECT_NAME}-${CMAKE_BUILD_TYPE}")

set(CPACK_DEBIAN_PACKAGE_HOMEPAGE     "https://github.com/piponazo/CppUtilities")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ library with general utilities")

set(CPACK_PACKAGE_VERSION             ${VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR       ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR       ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH       ${VERSION_PATCH})

set(CPACK_SOURCE_GENERATOR "TBZ2")
# TODO - Add git files
set(CPACK_SOURCE_IGNORE_FILES "\\\\.*.swp;/build_.*/;\\\\.hg/;\\\\.hgignore;\\\\.hgtags")

if (CPACK_GENERATOR STREQUAL "DEB")
   set(CPACK_DEB_COMPONENT_INSTALL ON)
   set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Luis Díaz Más piponazo@gmail.com")
   set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS OFF)
#   set(CPACK_DEBIAN_PACKAGE_DEPENDS "libargtable2-0 (>= 12-1)")
endif()

include(CPack)
