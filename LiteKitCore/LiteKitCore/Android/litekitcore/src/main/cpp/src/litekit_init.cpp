/*
 Copyright © 2020 Baidu, Inc. All Rights Reserved.

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <jni.h>
#include <string>
#include <map>

#include "common_log.h"
#include "litekit_init.h"
#include "litekit_inference_api.h"

namespace litekitcore {
using litekit_framework::LiteKitMachineService;
using litekit_framework::LiteKitConfig;

void paddleLiteConfigSetting(LiteKitConfig::PaddleLiteConfig &liteConfig,
                             int power_mode,
                             const std::string &modelPath) {
  liteConfig.powermode = static_cast<LiteKitConfig::PaddleLiteConfig::PaddleLitePowerMode>(power_mode);
  liteConfig.model_type = LiteKitConfig::PaddleLiteConfig::PaddleLiteModelType::LITE_MODEL_FROM_FILE;
  liteConfig.model.model_from_file.data = (char *) modelPath.c_str();
  liteConfig.model.model_from_file.size = modelPath.size();
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_baidu_litekitcore_LiteKitPaddleLiteMachine_createLiteKitMachineService(JNIEnv *env, jobject thiz,
                                                                    jint back_end_type,
                                                                    jstring model_file_path,
                                                                    jint model_type,
                                                                    jint thread,
                                                                    jint power_mode) {
  char *model_path = const_cast<char *>(env->GetStringUTFChars(model_file_path, JNI_FALSE));
  int length = env->GetStringLength(model_file_path);
  std::string modelPath = std::string(model_path, length);
  LiteKitMachineService* service = new LiteKitMachineService();
  service->autoRelease = true;

  LiteKitConfig::PaddleLiteConfig liteConfig;
  paddleLiteConfigSetting(liteConfig, power_mode, modelPath);

  LiteKitConfig litekitConfig = LiteKitConfig();
  litekitConfig.machine_config.paddle_lite_config = liteConfig;
  // 目前暂只支持paddlelite
  litekitConfig.machine_type = static_cast<LiteKitConfig::MachineType>(back_end_type);

  int result = service->load(litekitConfig);
  if (result != 0) {
    LOGE("service load error");
    return -1;
  }

  return reinterpret_cast<jlong>(service);
}

}
