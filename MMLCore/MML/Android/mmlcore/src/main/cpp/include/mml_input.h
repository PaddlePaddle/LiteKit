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

#ifndef MMLCORE_MML_FEED_INPUT_DATA_H
#define MMLCORE_MML_FEED_INPUT_DATA_H
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace mmlcore {

JNIEXPORT void JNICALL
feedFloatInputData(JNIEnv *env,
              jobject type,
              jlong nativeMachineHandler,
              jfloatArray data,
              jint inputBatch,
              jint inputChannel,
              jint inputHeight,
              jint inputWidth,
              jint inputGraphId);

JNIEXPORT void JNICALL
feedByteInputData(JNIEnv *env,
              jobject type,
              jlong nativeMachineHandler,
              jbyteArray data,
              jint inputBatch,
              jint inputChannel,
              jint inputHeight,
              jint inputWidth,
              jint inputGraphId);

JNIEXPORT void JNICALL
feedIntInputData(JNIEnv *env,
              jobject type,
              jlong nativeMachineHandler,
              jintArray data,
              jint inputBatch,
              jint inputChannel,
              jint inputHeight,
              jint inputWidth,
              jint inputGraphId);

JNIEXPORT void JNICALL
feedLongInputData(JNIEnv *env,
              jobject type,
              jlong nativeMachineHandler,
              jlongArray data,
              jint inputBatch,
              jint inputChannel,
              jint inputHeight,
              jint inputWidth,
              jint inputGraphId);

}

#ifdef __cplusplus
}
#endif

#endif //MMLCORE_MML_FEED_INPUT_DATA_H
