//
// Created by cuihao@baidu.com on 2020/9/2.
//

#include <jni.h>
#include "mml_input.h"

namespace mmlcore {
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
  const char *className = "com/baidu/mmlcore/MMLPaddleLiteMachine";
  jclass clazz = env->FindClass(className);
  if (clazz == NULL) {
    return result;
  }

  // 动态注册native方法
  if (env->RegisterNatives(clazz, mmlcore::methods, sizeof(mmlcore::methods) / sizeof(mmlcore::methods[0])) < 0) {
    return result;
  }

  // 返回成功
  result = JNI_VERSION_1_4;
  return result;
}