//
// Created by cuihao02@baidu.com on 2020/9/14.
//

#include <vector>

#include "mml_inference_api.h"
#include "mml_tensor.h"

namespace mmlcore {

extern "C"
JNIEXPORT jlongArray
JNICALL
Java_com_baidu_mmlcore_MMLTensor_fetchShape(JNIEnv *env,
                                            jobject thiz,
                                            jlong nativeTensorHandler) {
  mml_framework::MMLData *output = reinterpret_cast<mml_framework::MMLData *>(nativeTensorHandler);
  const auto &tensor = output->mmlTensor;
  std::vector<jlong> shape = tensor->shape();
  jlongArray ret = env->NewLongArray(shape.size());
  env->SetLongArrayRegion(ret, 0, shape.size(), shape.data());
  return ret;
}

extern "C"
JNIEXPORT jfloatArray
JNICALL
Java_com_baidu_mmlcore_MMLTensor_fetchFloatData(JNIEnv *env,
                                                jobject thiz,
                                                jlong nativeTensorHandler) {
  mml_framework::MMLData *output = reinterpret_cast<mml_framework::MMLData *>(nativeTensorHandler);
  const auto &tensor = output->mmlTensor;
  std::vector<int64_t> shape = tensor->shape();
  int64_t size = 1;
  for (int i = 0; i < shape.size(); ++i) {
    size *= shape.at(i);
  }
  jfloatArray ret = env->NewFloatArray(size);
  env->SetFloatArrayRegion(ret, 0, size, tensor->data<float>());
  return ret;
}

extern "C"
JNIEXPORT jintArray
JNICALL
Java_com_baidu_mmlcore_MMLTensor_fetchIntData(JNIEnv *env,
                                              jobject thiz,
                                              jlong nativeTensorHandler) {
  mml_framework::MMLData *output = reinterpret_cast<mml_framework::MMLData *>(nativeTensorHandler);
  const auto &tensor = output->mmlTensor;
  std::vector<int64_t> shape = tensor->shape();
  int64_t size = 1;
  for (int i = 0; i < shape.size(); ++i) {
    size *= shape.at(i);
  }
  jintArray ret = env->NewIntArray(size);
  env->SetIntArrayRegion(ret, 0, size, tensor->data<int>());
  return ret;
}

}

