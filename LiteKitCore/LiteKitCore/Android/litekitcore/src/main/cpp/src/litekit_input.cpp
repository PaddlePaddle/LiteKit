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
#include <vector>

#include "common_log.h"
#include "litekit_input.h"
#include "litekit_inference_api.h"

namespace litekitcore {
using litekit_framework::LiteKitMachineService;
using litekit_framework::LiteKitConfig;
using litekit_framework::LiteKitData;
using litekit_framework::LiteKitTensor;

JNIEXPORT void JNICALL
feedFloatInputData(JNIEnv *env, jobject type,
              jlong nativeMachineHandler, jfloatArray data,
              jint inputBatch, jint inputChannel,
              jint inputHeight, jint inputWidth,
              jint inputGraphId) {
  LiteKitMachineService *service = reinterpret_cast<LiteKitMachineService *>(nativeMachineHandler);
  std::unique_ptr<LiteKitData> input = service->getInputData(inputGraphId);
  LiteKitTensor *inputTensor = input->litekitTensor;
  inputTensor->Resize({inputBatch, inputChannel, inputHeight, inputWidth});
  float *inputData = inputTensor->mutable_data<float>();

  jfloat *idata = (jfloat *) env->GetFloatArrayElements(data, 0);
  jsize idataSize = env->GetArrayLength(data);
  std::copy(idata, idata + idataSize, inputData);
  env->ReleaseFloatArrayElements(data, idata, 0);
}


JNIEXPORT void JNICALL
feedByteInputData(JNIEnv *env, jobject type,
              jlong nativeMachineHandler, jbyteArray data,
              jint inputBatch, jint inputChannel,
              jint inputHeight, jint inputWidth,
              jint inputGraphId) {
  LiteKitMachineService *service = reinterpret_cast<LiteKitMachineService *>(nativeMachineHandler);
  std::unique_ptr<LiteKitData> input = service->getInputData(inputGraphId);
  LiteKitTensor *inputTensor = input->litekitTensor;
  inputTensor->Resize({inputBatch, inputChannel, inputHeight, inputWidth});
  jbyte *inputData = inputTensor->mutable_data<jbyte >();

  jbyte *idata = (jbyte *) env->GetByteArrayElements(data, 0);
  jsize idataSize = env->GetArrayLength(data);
  std::copy(idata, idata + idataSize, inputData);
  env->ReleaseByteArrayElements(data, idata, 0);
}

JNIEXPORT void JNICALL
feedIntInputData(JNIEnv *env, jobject type,
              jlong nativeMachineHandler, jintArray data,
              jint inputBatch, jint inputChannel,
              jint inputHeight, jint inputWidth,
              jint inputGraphId) {
  LiteKitMachineService *service = reinterpret_cast<LiteKitMachineService *>(nativeMachineHandler);
  std::unique_ptr<LiteKitData> input = service->getInputData(inputGraphId);
  LiteKitTensor *inputTensor = input->litekitTensor;
  inputTensor->Resize({inputBatch, inputChannel, inputHeight, inputWidth});
  auto inputData = inputTensor->mutable_data<int>();

  auto idata = env->GetIntArrayElements(data, 0);
  jsize idataSize = env->GetArrayLength(data);
  std::copy(idata, idata + idataSize, inputData);
  env->ReleaseIntArrayElements(data, idata, 0);
}

JNIEXPORT void JNICALL
feedLongInputData(JNIEnv *env, jobject type,
              jlong nativeMachineHandler, jlongArray data,
              jint inputBatch, jint inputChannel,
              jint inputHeight, jint inputWidth,
              jint inputGraphId) {
  LiteKitMachineService *service = reinterpret_cast<LiteKitMachineService *>(nativeMachineHandler);
  std::unique_ptr<LiteKitData> input = service->getInputData(inputGraphId);
  LiteKitTensor *inputTensor = input->litekitTensor;
  inputTensor->Resize({inputBatch, inputChannel, inputHeight, inputWidth});
  jlong *inputData = inputTensor->mutable_data<jlong>();

  jlong *idata = (jlong *) env->GetLongArrayElements(data, 0);
  jsize idataSize = env->GetArrayLength(data);
  std::copy(idata, idata + idataSize, inputData);
  env->ReleaseLongArrayElements(data, idata, 0);
}
}

