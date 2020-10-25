//
// Created by cuihao02@baidu.com on 2020/8/24.
//

#ifndef MMLCORE_MMLCORE_SRC_MAIN_CPP_INCLUDE_MML_OUTPUT_H_
#define MMLCORE_MMLCORE_SRC_MAIN_CPP_INCLUDE_MML_OUTPUT_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace mmlcore {

extern "C"
JNIEXPORT jint
JNICALL
Java_com_baidu_mmlcore_MMLPaddleLiteMachine_fetchOutputCount(JNIEnv *env,
                                                             jobject thiz,
                                                             jlong nativeMachineHandler);

extern "C"
JNIEXPORT jlong
JNICALL
Java_com_baidu_mmlcore_MMLPaddleLiteMachine_fetchOutputMMLTensor(JNIEnv *env,
                                                                 jobject thiz,
                                                                 jlong nativeMachineHandler,
                                                                 jint outputGraphId);
}

#ifdef __cplusplus
}
#endif

#endif //MMLCORE_MMLCORE_SRC_MAIN_CPP_INCLUDE_MML_OUTPUT_H_
