//
// Created by acui on 2020/8/24.
//

#include <jni.h>
#include <string>
#include <map>

#include "common_log.h"
#include "mml_release.h"
#include "mml_inference_api.h"

namespace mmlcore {
using mml_framework::MMLMachineService;
using mml_framework::MMLConfig;

extern "C"
JNIEXPORT void JNICALL
Java_com_baidu_mmlcore_MMLBaseMachine_release(JNIEnv
                                                    *env,
                                                    jobject thiz, jlong
                                                    nativeMachineHandler) {
  MMLMachineService* service = reinterpret_cast<MMLMachineService*>(nativeMachineHandler);
  delete service;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_baidu_mmlcore_MMLTensor_release(JNIEnv *env, jobject thiz, jlong nativeTensorHandler) {
  mml_framework::MMLData *output = reinterpret_cast<mml_framework::MMLData*>(nativeTensorHandler);
  delete output;
}

}