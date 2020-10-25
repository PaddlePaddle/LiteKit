//
// Created by acui on 2020/8/24.
//

#ifndef MMLCORE_MMLCORE_SRC_MAIN_CPP_INCLUDE_MML_RELEASE_H_
#define MMLCORE_MMLCORE_SRC_MAIN_CPP_INCLUDE_MML_RELEASE_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace mmlcore {

extern "C"
JNIEXPORT void JNICALL
Java_com_baidu_mmlcore_MMLBaseMachine_release(JNIEnv *env, jobject thiz, jlong nativeMachineHandler);

extern "C"
JNIEXPORT void JNICALL
Java_com_baidu_mmlcore_MMLTensor_release(JNIEnv *env, jobject thiz, jlong nativeTensorHandler);

}

#ifdef __cplusplus
}
#endif

#endif //MMLCORE_MMLCORE_SRC_MAIN_CPP_INCLUDE_MML_RELEASE_H_
