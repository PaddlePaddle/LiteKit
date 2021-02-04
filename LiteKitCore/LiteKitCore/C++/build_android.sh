#!/bin/bash
# Copyright © 2020 Baidu, Inc. All Rights Reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
# WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


# First of all modify `ANDROID_CMAKE`, `ANDROID_NDK` to your own in ~/.bashrc file, then
# bash
# sh build_android.sh

echo "android cmake: $ANDROID_CMAKE"
echo "android ndk: $ANDROID_NDK"
export ANDROID_SYSROOT=${ANDROID_NDK}/sysroot

# get absolute path
work_path=$(cd `dirname $0`; pwd)
echo "work_path:$work_path"

# -------------
# cpu && gpu v8a
# -------------
cd $work_path
rm -rf android_release_v8a
mkdir android_release_v8a
cd android_release_v8a

$ANDROID_CMAKE .. \
    -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
     -DANDROID_TOOLCHAIN_NAME=llvm \
    -DANDROID_TOOLCHAIN=clang \
    -DCMAKE_BUILD_TYPE=Release \
    -DANDROID_ABI="arm64-v8a" \
    -DANDROID_ARM_NEON=ON \
    -DANDROID_STL=c++_shared \
    -DANDROID_NATIVE_API_LEVEL=android-19

make -j4

# -------------
# cpu && gpu v7a
# -------------
cd $work_path
rm -rf android_release_v7a
mkdir android_release_v7a
cd android_release_v7a

$ANDROID_CMAKE .. \
    -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_TOOLCHAIN_NAME=llvm \
    -DANDROID_TOOLCHAIN=clang \
    -DCMAKE_BUILD_TYPE=Release \
    -DANDROID_ABI="armeabi-v7a" \
    -DANDROID_ARM_NEON=ON \
    -DANDROID_STL=c++_shared \
    -DANDROID_NATIVE_API_LEVEL=android-19

make -j4

cd $work_path
rm -rf output
mkdir -p output/armeabi-v7a
mkdir -p output/arm64-v8a

cp -r $work_path/android_release_v7a/src/main/cpp/litekit_framework/liblitekit_framework.so output/armeabi-v7a/
cp -r $work_path/android_release_v8a/src/main/cpp/litekit_framework/liblitekit_framework.so output/arm64-v8a/
cp -r src/main/cpp/litekit_framework/litekit_inference_api.h output/litekit_inference_api.h

cd $work_path
rm -rf android_release_v7a
rm -rf android_release_v8a

echo "----------------------------"
echo "Build successfully, enjoy it"
echo "----------------------------"
