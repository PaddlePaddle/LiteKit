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

#ifndef MMLCORE_APP_SRC_MAIN_CPP_INCLUDE_POSTPROCESS_H_
#define MMLCORE_APP_SRC_MAIN_CPP_INCLUDE_POSTPROCESS_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace imba {

extern "C"
JNIEXPORT jfloatArray JNICALL
Java_com_baidu_mml_demo_MainActivity_postprocess(JNIEnv *env,
                                                 jobject thiz,
                                                 jfloatArray data0,
                                                 jfloatArray data1,
                                                 jfloatArray data2,
                                                 jfloatArray data3,
                                                 jfloatArray data4,
                                                 jint imgWidth,
                                                 jint imgHeight
);

}

#ifdef __cplusplus
}
#endif

#endif //MMLCORE_APP_SRC_MAIN_CPP_INCLUDE_POSTPROCESS_H_
