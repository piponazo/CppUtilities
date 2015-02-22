INCLUDE(CMakeForceCompiler)

SET(CMAKE_SYSTEM_NAME Linux) # this one is important
SET(CMAKE_SYSTEM_VERSION 1)  # this one not so much

# specify the cross compiler
SET(RPI_ROOT /home/luis/programming/rpi-toolchain/arm-bcm2708/arm-bcm2708hardfp-linux-gnueabi)

SET(CMAKE_C_COMPILER   ${RPI_ROOT}/bin/arm-bcm2708hardfp-linux-gnueabi-gcc)
SET(CMAKE_CXX_COMPILER ${RPI_ROOT}/bin/arm-bcm2708hardfp-linux-gnueabi-g++)
set(CMAKE_AR           ${RPI_ROOT}/bin/arm-bcm2708hardfp-linux-gnueabi-ar)
set(CMAKE_LINKER       ${RPI_ROOT}/bin/arm-bcm2708hardfp-linux-gnueabi-ld)
set(CMAKE_NM           ${RPI_ROOT}/bin/arm-bcm2708hardfp-linux-gnueabi-nm)
set(CMAKE_OBJCOPY      ${RPI_ROOT}/bin/arm-bcm2708hardfp-linux-gnueabi-objcopy)
set(CMAKE_OBJDUMP      ${RPI_ROOT}/bin/arm-bcm2708hardfp-linux-gnueabi-objdump)
set(CMAKE_STRIP        ${RPI_ROOT}/bin/arm-bcm2708hardfp-linux-gnueabi-strip)
set(CMAKE_RANLIB       ${RPI_ROOT}/bin/arm-bcm2708hardfp-linux-gnueabi-tanlib)

# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  ${RPI_ROOT}/arm-bcm2708hardfp-linux-gnueabi)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
