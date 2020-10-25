//
// Created by cuihao02@baidu.com on 2020/9/14.
//

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
