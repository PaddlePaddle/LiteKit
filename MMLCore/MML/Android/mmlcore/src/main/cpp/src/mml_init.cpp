//
// Created by cuihao02@baidu.com on 2020/8/21.
//

#include <jni.h>
#include <string>
#include <map>

#include "common_log.h"
#include "mml_init.h"
#include "mml_inference_api.h"

namespace mmlcore {
using mml_framework::MMLMachineService;
using mml_framework::MMLConfig;

void paddleLiteConfigSetting(MMLConfig::PaddleLiteConfig &liteConfig,
                             int power_mode,
                             const std::string &modelPath) {
  liteConfig.powermode = static_cast<MMLConfig::PaddleLiteConfig::PaddleLitePowerMode>(power_mode);
  liteConfig.model_type = MMLConfig::PaddleLiteConfig::PaddleLiteModelType::LITE_MODEL_FROM_FILE;
  liteConfig.model.model_from_file.data = (char *) modelPath.c_str();
  liteConfig.model.model_from_file.size = modelPath.size();
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_baidu_mmlcore_MMLPaddleLiteMachine_createMMLMachineService(JNIEnv *env, jobject thiz,
                                                                    jint back_end_type,
                                                                    jstring model_file_path,
                                                                    jint model_type,
                                                                    jint thread,
                                                                    jint power_mode) {
  char *model_path = const_cast<char *>(env->GetStringUTFChars(model_file_path, JNI_FALSE));
  int length = env->GetStringLength(model_file_path);
  std::string modelPath = std::string(model_path, length);
  MMLMachineService* service = new MMLMachineService();
  service->autoRelease = true;

  MMLConfig::PaddleLiteConfig liteConfig;
  paddleLiteConfigSetting(liteConfig, power_mode, modelPath);

  MMLConfig mmlConfig = MMLConfig();
  mmlConfig.machine_config.paddle_lite_config = liteConfig;
  // 目前暂只支持paddlelite
  mmlConfig.machine_type = static_cast<MMLConfig::MachineType>(back_end_type);

  int result = service->load(mmlConfig);
  if (result != 0) {
    LOGE("service load error");
    return -1;
  }

  return reinterpret_cast<jlong>(service);
}

}
