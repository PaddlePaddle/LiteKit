//
// Created by cuihao02@baidu.com on 2020/8/21.
//

/* DO NOT EDIT THIS FILE - it is machine generated */
#ifndef MMLCORE_MML_INIT_H
#define MMLCORE_MML_INIT_H

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace mmlcore {

extern "C"
JNIEXPORT jlong JNICALL
Java_com_baidu_mmlcore_MMLPaddleLiteMachine_createMMLMachineService(JNIEnv *env,
                                                                    jobject thiz,
                                                                    jint back_end_type,
                                                                    jstring model_path,
                                                                    jint model_type,
                                                                    jint thread,
                                                                    jint power_mode);

}

#ifdef __cplusplus
}
#endif
#endif