#/bin/sh -f

SOURCE_DIR=`pwd`
BUILD_DIR=build

if [ "$CC" == "gcc" ]; then
    CLANG=OFF
else
    CLANG=ON
fi

mkdir build && cd build
cmake $SOURCE_DIR -DBUILD_TESTS=ON -DUSE_CLANG=$CLANG
