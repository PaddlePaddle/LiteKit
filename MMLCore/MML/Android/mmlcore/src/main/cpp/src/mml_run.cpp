//
// Created by cuihao02@baidu.com on 2020/8/22.
//

#include <jni.h>
#include <string>
#include <map>

#include "common_log.h"
#include "mml_run.h"
#include "mml_inference_api.h"

namespace mmlcore {
using mml_framework::MMLMachineService;
using mml_framework::MMLConfig;

extern "C"
JNIEXPORT void JNICALL
Java_com_baidu_mmlcore_MMLBaseMachine_run(JNIEnv *env, jobject type, jlong nativeMachineHandler) {
  MMLMachineService* service = reinterpret_cast<MMLMachineService*>(nativeMachineHandler);
  int ret = service->run();
  if (ret != 0) {
    LOGE("service run error");
  }
}

}
