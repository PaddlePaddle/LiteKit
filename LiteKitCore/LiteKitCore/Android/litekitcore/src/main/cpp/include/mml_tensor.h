// Copyright (c) 2019 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef ANDROID_MMLCORE_SRC_MAIN_CPP_INCLUDE_MML_TENSOR_H_
#define ANDROID_MMLCORE_SRC_MAIN_CPP_INCLUDE_MML_TENSOR_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace mmlcore {

extern "C"
JNIEXPORT jlongArray
JNICALL
Java_com_baidu_mmlcore_MMLTensor_fetchShape(JNIEnv *env,
                                                  jobject thiz,
                                                  jlong nativeTensorHandler);

extern "C"
JNIEXPORT jfloatArray
JNICALL
Java_com_baidu_mmlcore_MMLTensor_fetchFloatData(JNIEnv *env,
                                              jobject thiz,
                                              jlong nativeTensorHandler);

extern "C"
JNIEXPORT jintArray
JNICALL
Java_com_baidu_mmlcore_MMLTensor_fetchIntData(JNIEnv *env,
                                              jobject thiz,
                                              jlong nativeTensorHandler);
}

#ifdef __cplusplus
}
#endif

#endif //ANDROID_MMLCORE_SRC_MAIN_CPP_INCLUDE_MML_TENSOR_H_
