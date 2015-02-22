INCLUDE(CMakeForceCompiler)

SET(CMAKE_SYSTEM_NAME Linux) # this one is important
SET(CMAKE_SYSTEM_VERSION 1)  # this one not so much

SET(CMAKE_C_COMPILER   /usr/bin/arm-linux-gnueabi-gcc-4.6)
SET(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabi-g++-4.6)
set(CMAKE_AR           /usr/bin/arm-linux-gnueabi-ar)
set(CMAKE_LINKER       /usr/bin/arm-linux-gnueabi-ld)
set(CMAKE_NM           /usr/bin/arm-linux-gnueabi-nm)
set(CMAKE_OBJCOPY      /usr/bin/arm-linux-gnueabi-objcopy)
set(CMAKE_OBJDUMP      /usr/bin/arm-linux-gnueabi-objdump)
set(CMAKE_STRIP        /usr/bin/arm-linux-gnueabi-strip)
set(CMAKE_RANLIB       /usr/bin/arm-linux-gnueabi-tanlib)

SET(CMAKE_FIND_ROOT_PATH  $ENV{HOME}/arm-linux-gnueabi/)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
