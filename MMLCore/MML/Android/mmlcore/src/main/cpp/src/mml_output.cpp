//
// Created by cuihao02@baidu.com on 2020/8/24.
//
#include <jni.h>
#include <string>
#include <map>
#include <vector>

#include "common_log.h"
#include "mml_output.h"
#include "mml_inference_api.h"

namespace mmlcore {
using mml_framework::MMLMachineService;
using mml_framework::MMLConfig;

extern "C"
JNIEXPORT jint
JNICALL
Java_com_baidu_mmlcore_MMLPaddleLiteMachine_fetchOutputCount(JNIEnv *env,
                                                             jobject thiz,
                                                             jlong nativeMachineHandler) {
  MMLMachineService* service = reinterpret_cast<MMLMachineService*>(nativeMachineHandler);
  return service->getOutputNames().size();
}

extern "C"
JNIEXPORT jlong
JNICALL
Java_com_baidu_mmlcore_MMLPaddleLiteMachine_fetchOutputMMLTensor(JNIEnv *env,
                                                                 jobject thiz,
                                                                 jlong nativeMachineHandler, jint outputGraphId) {
  MMLMachineService* service = reinterpret_cast<MMLMachineService*>(nativeMachineHandler);
  std::unique_ptr<const mml_framework::MMLData> output = service->getOutputData(outputGraphId);
  return reinterpret_cast<jlong>(output.release());
}


}
