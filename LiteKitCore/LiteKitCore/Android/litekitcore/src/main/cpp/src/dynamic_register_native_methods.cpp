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
