#/bin/sh -f

SOURCE_DIR=`pwd`
BUILD_DIR=build
cd build

if [ "$CXX" == "g++" ]; then
    cmake $SOURCE_DIR -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON
else
    cmake $SOURCE_DIR -DBUILD_TESTS=ON -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++
fi

