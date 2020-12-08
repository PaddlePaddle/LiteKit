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

