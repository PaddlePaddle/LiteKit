//
// Created by cuihao02@baidu.com on 2020/8/22.
//
#include <jni.h>
#include <string>
#include <map>
#include <vector>

#include "common_log.h"
#include "mml_input.h"
#include "mml_inference_api.h"

namespace mmlcore {
using mml_framework::MMLMachineService;
using mml_framework::MMLConfig;
using mml_framework::MMLData;
using mml_framework::MMLTensor;

JNIEXPORT void JNICALL
feedFloatInputData(JNIEnv *env, jobject type,
              jlong nativeMachineHandler, jfloatArray data,
              jint inputBatch, jint inputChannel,
              jint inputHeight, jint inputWidth,
              jint inputGraphId) {
  MMLMachineService *service = reinterpret_cast<MMLMachineService *>(nativeMachineHandler);
  std::unique_ptr<MMLData> input = service->getInputData(inputGraphId);
  MMLTensor *inputTensor = input->mmlTensor;
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
  MMLMachineService *service = reinterpret_cast<MMLMachineService *>(nativeMachineHandler);
  std::unique_ptr<MMLData> input = service->getInputData(inputGraphId);
  MMLTensor *inputTensor = input->mmlTensor;
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
  MMLMachineService *service = reinterpret_cast<MMLMachineService *>(nativeMachineHandler);
  std::unique_ptr<MMLData> input = service->getInputData(inputGraphId);
  MMLTensor *inputTensor = input->mmlTensor;
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
  MMLMachineService *service = reinterpret_cast<MMLMachineService *>(nativeMachineHandler);
  std::unique_ptr<MMLData> input = service->getInputData(inputGraphId);
  MMLTensor *inputTensor = input->mmlTensor;
  inputTensor->Resize({inputBatch, inputChannel, inputHeight, inputWidth});
  jlong *inputData = inputTensor->mutable_data<jlong>();

  jlong *idata = (jlong *) env->GetLongArrayElements(data, 0);
  jsize idataSize = env->GetArrayLength(data);
  std::copy(idata, idata + idataSize, inputData);
  env->ReleaseLongArrayElements(data, idata, 0);
}
}

