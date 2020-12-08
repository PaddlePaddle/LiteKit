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
#include "litekit_output.h"
#include "litekit_inference_api.h"

namespace litekitcore {
using litekit_framework::LiteKitMachineService;
using litekit_framework::LiteKitConfig;

extern "C"
JNIEXPORT jint
JNICALL
Java_com_baidu_litekitcore_LiteKitPaddleLiteMachine_fetchOutputCount(JNIEnv *env,
                                                             jobject thiz,
                                                             jlong nativeMachineHandler) {
  LiteKitMachineService* service = reinterpret_cast<LiteKitMachineService*>(nativeMachineHandler);
  return service->getOutputNames().size();
}

extern "C"
JNIEXPORT jlong
JNICALL
Java_com_baidu_litekitcore_LiteKitPaddleLiteMachine_fetchOutputLiteKitTensor(JNIEnv *env,
                                                                 jobject thiz,
                                                                 jlong nativeMachineHandler, jint outputGraphId) {
  LiteKitMachineService* service = reinterpret_cast<LiteKitMachineService*>(nativeMachineHandler);
  std::unique_ptr<const litekit_framework::LiteKitData> output = service->getOutputData(outputGraphId);
  return reinterpret_cast<jlong>(output.release());
}


}
