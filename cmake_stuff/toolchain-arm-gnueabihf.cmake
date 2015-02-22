INCLUDE(CMakeForceCompiler)

SET(CMAKE_SYSTEM_NAME Linux) # this one is important
SET(CMAKE_SYSTEM_VERSION 1)  # this one not so much

# specify the cross compiler
SET(CMAKE_C_COMPILER   /usr/bin/arm-linux-gnueabihf-gcc-4.6)
SET(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++-4.6)
set(CMAKE_AR           /usr/bin/arm-linux-gnueabihf-ar)
set(CMAKE_LINKER       /usr/bin/arm-linux-gnueabihf-ld)
set(CMAKE_NM           /usr/bin/arm-linux-gnueabihf-nm)
set(CMAKE_OBJCOPY      /usr/bin/arm-linux-gnueabihf-objcopy)
set(CMAKE_OBJDUMP      /usr/bin/arm-linux-gnueabihf-objdump)
set(CMAKE_STRIP        /usr/bin/arm-linux-gnueabihf-strip)
set(CMAKE_RANLIB       /usr/bin/arm-linux-gnueabihf-tanlib)

# where is the target environment 
SET(BB_PATH /usr/arm-linux-gnueabihf)
SET(CMAKE_FIND_ROOT_PATH  ${BB_PATH})

# search for programs in the build host directories
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# FindQt4.cmake querys qmake to get information, this doesn't work when crosscompiling
SET(QT_LIBRARY_DIR			${BB_PATH}/lib)
SET(QT_QTCORE_LIBRARIES		${BB_PATH}/lib/libQtCore.so)
SET(QT_INCLUDE_DIR			${BB_PATH}/include/qt4)
#SET(QT_MKSPECS_DIR			${BB_PATH}/mkspecs)
#SET(QT_MOC_EXECUTABLE		${BB_PATH}/moc)
#SET(QT_QMAKE_EXECUTABLE	${BB_PATH}/qmake)
#SET(QT_UIC_EXECUTABLE		${BB_PATH}/uic)
