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

#include <vector>

#include "litekit_inference_api.h"
#include "litekit_tensor.h"

namespace litekitcore {

extern "C"
JNIEXPORT jlongArray
JNICALL
Java_com_baidu_litekitcore_LiteKitTensor_fetchShape(JNIEnv *env,
                                            jobject thiz,
                                            jlong nativeTensorHandler) {
  litekit_framework::LiteKitData *output = reinterpret_cast<litekit_framework::LiteKitData *>(nativeTensorHandler);
  const auto &tensor = output->litekitTensor;
  std::vector<jlong> shape = tensor->shape();
  jlongArray ret = env->NewLongArray(shape.size());
  env->SetLongArrayRegion(ret, 0, shape.size(), shape.data());
  return ret;
}

extern "C"
JNIEXPORT jfloatArray
JNICALL
Java_com_baidu_litekitcore_LiteKitTensor_fetchFloatData(JNIEnv *env,
                                                jobject thiz,
                                                jlong nativeTensorHandler) {
  litekit_framework::LiteKitData *output = reinterpret_cast<litekit_framework::LiteKitData *>(nativeTensorHandler);
  const auto &tensor = output->litekitTensor;
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
Java_com_baidu_litekitcore_LiteKitTensor_fetchIntData(JNIEnv *env,
                                              jobject thiz,
                                              jlong nativeTensorHandler) {
  litekit_framework::LiteKitData *output = reinterpret_cast<litekit_framework::LiteKitData *>(nativeTensorHandler);
  const auto &tensor = output->litekitTensor;
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

