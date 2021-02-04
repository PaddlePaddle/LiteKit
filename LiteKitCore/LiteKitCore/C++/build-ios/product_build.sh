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

CURRENT_DIR=`pwd`
BUILD_DIR_NAME=build
### outside path , must be absolute path
# BUILD_DIR=xxxxxx/${BUILD_DIR_NAME}
### internal path , must be absolute path
BUILD_DIR=$CURRENT_DIR/${BUILD_DIR_NAME}
rm -r ${BUILD_DIR}
mkdir -p ${BUILD_DIR}
echo "###############################################################"
echo "**** 即将开始编译iOS native arm全架构(arm64+armv7)版本的litekit库 ****"
echo "**** 将要编译产物到目录：${BUILD_DIR} ****"
echo "###############################################################"
sleep 0.5

ARM64=arm64
ARMV7=armv7
IOS_ARCHITECTURE=(armv7 arm64)
PRODUCT_LIB=liblitekit_framework_ios.a
TOOL_CHAIN_FILE=${CURRENT_DIR}/tools/iOS.cmake
MODE="Release"

#############################
###### build for arm64 ######
#############################
cmake \
    -B"${BUILD_DIR}" \
    -DCMAKE_BUILD_TYPE=${MODE} \
    -DCMAKE_TOOLCHAIN_FILE=${TOOL_CHAIN_FILE} \
    -DIOS_PLATFORM=OS \
    -DCMAKE_OSX_ARCHITECTURES=$ARM64 \
    -DIS_IOS="true"

cd ${BUILD_DIR}
make

###### copy prudoct
PRODUCT_DIR=${BUILD_DIR}/../product

PRODUCT_TEMP_DIR=$PRODUCT_DIR/temp_arch
PRODUCT_HEADER_DIR=$PRODUCT_DIR/include
PRODUCT_ARM_FINAL_DIR=$PRODUCT_DIR/arm_product
PRODUCT_FINAL_DIR=$PRODUCT_DIR/release

cd ${BUILD_DIR}/.. && mkdir $PRODUCT_DIR
mkdir -p $PRODUCT_HEADER_DIR
mkdir -p $PRODUCT_ARM_FINAL_DIR
mkdir -p $PRODUCT_TEMP_DIR/$ARM64


cp ${CURRENT_DIR}/../src/main/cpp/litekit_framework/litekit_inference_api.h $PRODUCT_HEADER_DIR/litekit_inference_api.h
cp ${CURRENT_DIR}/../src/main/cpp/litekit_framework/paddle_lite/paddle_lite_image_preprocess.h $PRODUCT_HEADER_DIR/paddle_lite_image_preprocess.h
mv $BUILD_DIR/$PRODUCT_LIB $PRODUCT_TEMP_DIR/$ARM64/$PRODUCT_LIB


#############################
###### build for armv7 ######
#############################
# rm "cmake_install.cmake" | rm "CMakeCache.txt" | rm -r CMakeFiles
rm -r ${BUILD_DIR}
cd $CURRENT_DIR
cmake \
    -B"${BUILD_DIR}" \
    -DCMAKE_BUILD_TYPE=${MODE} \
    -DCMAKE_TOOLCHAIN_FILE=${TOOL_CHAIN_FILE} \
    -DIOS_PLATFORM=OS \
    -DCMAKE_OSX_ARCHITECTURES=$ARMV7 \
    -DIS_IOS="true"

cd ${BUILD_DIR}
make

###### move file
cd ${BUILD_DIR}/..
mkdir -p $PRODUCT_TEMP_DIR/$ARMV7
mv $BUILD_DIR/$PRODUCT_LIB $PRODUCT_TEMP_DIR/$ARMV7/$PRODUCT_LIB

mkdir -p $PRODUCT_FINAL_DIR
#### merge
PRODUCT_LIB_TINY=liblitekit_framework_ios_tiny.a
lipo -create $PRODUCT_TEMP_DIR/$ARMV7/$PRODUCT_LIB $PRODUCT_TEMP_DIR/$ARM64/$PRODUCT_LIB -output $PRODUCT_FINAL_DIR/$PRODUCT_LIB_TINY

xcrun -r libtool -static -o $PRODUCT_FINAL_DIR/$PRODUCT_LIB \
        $PRODUCT_FINAL_DIR/$PRODUCT_LIB_TINY \
        $PRODUCT_DIR/../../src/main/iOSLibs/paddle_api_light_bundled/libpaddle_api_light_bundled.a

#### move to release & delete temp files
cp -r $PRODUCT_HEADER_DIR $PRODUCT_FINAL_DIR
rm -r $PRODUCT_ARM_FINAL_DIR  | rm -r $PRODUCT_HEADER_DIR | rm -r $PRODUCT_TEMP_DIR | rm -r $BUILD_DIR
