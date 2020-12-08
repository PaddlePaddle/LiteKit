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
#include "litekit_input.h"

namespace litekitcore {
// 参数映射表
static JNINativeMethod methods[] = {
    {"feedInputData", "(J[FIIIII)V", (void *) feedFloatInputData},
    {"feedInputData", "(J[BIIIII)V", (void *) feedByteInputData},
    {"feedInputData", "(J[IIIIII)V", (void *) feedIntInputData},
    {"feedInputData", "(J[JIIIII)V", (void *) feedLongInputData},
};
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
  JNIEnv *env = NULL;
  jint result = -1;

  // 获取JNI env变量
  if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
    // 失败返回-1
    return result;
  }

  // 获取native方法所在类
  const char *className = "com/baidu/litekitcore/LiteKitPaddleLiteMachine";
  jclass clazz = env->FindClass(className);
  if (clazz == NULL) {
    return result;
  }

  // 动态注册native方法
  if (env->RegisterNatives(clazz, litekitcore::methods, sizeof(litekitcore::methods) / sizeof(litekitcore::methods[0])) < 0) {
    return result;
  }

  // 返回成功
  result = JNI_VERSION_1_4;
  return result;
}
