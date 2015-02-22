INCLUDE(CMakeForceCompiler)

SET(CMAKE_SYSTEM_NAME Linux) # this one is important
SET(CMAKE_SYSTEM_VERSION 1)  # this one not so much
set(CMAKE_SYSTEM_PROCESSOR arm)

SET(TOOLCHAIN_PATH /opt/linaro-toolchain483)

# specify the cross compiler
SET(CMAKE_C_COMPILER   ${TOOLCHAIN_PATH}/bin/arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/bin/arm-linux-gnueabihf-g++)
set(CMAKE_AR           ${TOOLCHAIN_PATH}/bin/arm-linux-gnueabihf-ar)
set(CMAKE_LINKER       ${TOOLCHAIN_PATH}/bin/arm-linux-gnueabihf-ld)
set(CMAKE_NM           ${TOOLCHAIN_PATH}/bin/arm-linux-gnueabihf-nm)
set(CMAKE_OBJCOPY      ${TOOLCHAIN_PATH}/bin/arm-linux-gnueabihf-objcopy)
set(CMAKE_OBJDUMP      ${TOOLCHAIN_PATH}/bin/arm-linux-gnueabihf-objdump)
set(CMAKE_STRIP        ${TOOLCHAIN_PATH}/bin/arm-linux-gnueabihf-strip)
set(CMAKE_RANLIB       ${TOOLCHAIN_PATH}/bin/arm-linux-gnueabihf-tanlib)

# where is the target environment 
SET(BB_PATH	/usr/arm-linux-gnueabihf)
SET(CMAKE_FIND_ROOT_PATH	${CMAKE_FIND_ROOT_PATH} ${BB_PATH})

# search for programs in the build host directories
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# ARGTABLE
INCLUDE_DIRECTORIES(SYSTEM ${BB_PATH}/include/argtable)

# FindQt4.cmake querys qmake to get information, this doesn't work when crosscompiling
SET(QT_LIBRARY_DIR		   ${BB_PATH}/lib)
SET(QT_QTCORE_LIBRARIES	   ${BB_PATH}/lib/libQtCore.so)
SET(QT_INCLUDE_DIR		   ${BB_PATH}/include/qt4)
SET(QT_QTCORE_INCLUDE_DIR	${BB_PATH}/include/qt4/QtCore)
