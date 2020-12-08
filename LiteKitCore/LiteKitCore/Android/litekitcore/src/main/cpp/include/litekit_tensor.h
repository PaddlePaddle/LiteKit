/*
 Copyright © 2020 Baidu, Inc. All Rights Reserved.

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef ANDROID_LITEKITCORE_SRC_MAIN_CPP_INCLUDE_LITEKIT_TENSOR_H_
#define ANDROID_LITEKITCORE_SRC_MAIN_CPP_INCLUDE_LITEKIT_TENSOR_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace litekitcore {

extern "C"
JNIEXPORT jlongArray
JNICALL
Java_com_baidu_litekitcore_LiteKitTensor_fetchShape(JNIEnv *env,
                                                  jobject thiz,
                                                  jlong nativeTensorHandler);

extern "C"
JNIEXPORT jfloatArray
JNICALL
Java_com_baidu_litekitcore_LiteKitTensor_fetchFloatData(JNIEnv *env,
                                              jobject thiz,
                                              jlong nativeTensorHandler);

extern "C"
JNIEXPORT jintArray
JNICALL
Java_com_baidu_litekitcore_LiteKitTensor_fetchIntData(JNIEnv *env,
                                              jobject thiz,
                                              jlong nativeTensorHandler);
}

#ifdef __cplusplus
}
#endif

#endif //ANDROID_LITEKITCORE_SRC_MAIN_CPP_INCLUDE_LITEKIT_TENSOR_H_
