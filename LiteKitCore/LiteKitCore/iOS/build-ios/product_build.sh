#!/bin/bash

pod install

set -x

SCHEME=LiteKitCore

IOS_LIBS_DIR=../Dependences
XCODE_WORKSPACE_NAME=LiteKitCore.xcworkspace
XCODE_PROJECT_DIR=.
BUILD_DIR=${XCODE_PROJECT_DIR}/build
DEVICE_BUILD=${BUILD_DIR}/device_build_intermediate
SIMULATOR_BUILD=${BUILD_DIR}/simulator_build_intermediate
DEVICE_BUILD_PRODUCT=${DEVICE_BUILD}/Build/Products/Release-iphoneos
HEADER_PRODUCT=${DEVICE_BUILD}/Build/Products/Release-iphoneos/usr/local/include
OUTPUT_DIR=./output
FRAMEWORK_NAME=${SCHEME}.framework
PRODUCT_NAME=${SCHEME}.framework

echo "clean output dir"
rm -rf ${OUTPUT_DIR}
mkdir ${OUTPUT_DIR}

echo "clean intermediate dir"
rm -rf ${BUILD_DIR}
mkdir ${BUILD_DIR}
mkdir ${DEVICE_BUILD}
mkdir ${SIMULATOR_BUILD}

xcodebuild -workspace ${XCODE_PROJECT_DIR}/${XCODE_WORKSPACE_NAME} -scheme ${SCHEME} ONLY_ACTIVE_ARCH=NO -configuration 'Release' -sdk iphoneos clean build -derivedDataPath ${DEVICE_BUILD} OTHER_CFLAGS="-fembed-bitcode"

#cp -r ${HEADER_PRODUCT} ${OUTPUT_DIR}
cp -rf ${DEVICE_BUILD_PRODUCT}/${FRAMEWORK_NAME} ${OUTPUT_DIR}/${PRODUCT_NAME}
rm -rf ${BUILD_DIR}/
