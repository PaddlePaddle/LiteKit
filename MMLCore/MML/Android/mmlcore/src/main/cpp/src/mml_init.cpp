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
