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

