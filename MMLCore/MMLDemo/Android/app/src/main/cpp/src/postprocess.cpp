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

#include <cmath>
#include "postprocess.h"

namespace imba {

bool classTh(int index, float conf) {
  int cls = index % 6;
  bool res = false;
  switch (cls) {
    case 0:
      if (conf > 0.2) {
        res = true;
      }
      break;
    case 1:
      if (conf > 0.3) {
        res = true;
      }
      break;
    case 2:
      if (conf > 0.3) {
        res = true;
      }
      break;
    case 3:
      if (conf > 0.4) {
        res = true;
      }
      break;
    case 4:
      if (conf > 0.3) {
        res = true;
      }
      break;
    case 5:
      if (conf > 0.3) {
        res = true;
      }
      break;
    default:break;
  }
  return res;
}

int getMaxIndex(const float *classData, int len) {
  int maxIndex = -1;
  float maxConf = -1.0;
  float curConf = -2.0;
  for (int i = 0; i < len; i++) {
    curConf = classData[i];
    if (curConf > 0.4 && curConf > maxConf && classTh(i, curConf)) {
      maxConf = curConf;
      maxIndex = i;
    }
  }
  return maxIndex;
}

void postprocess(float *result, float *outputs[], float originWidth, float originHeight) {
  int inputWidth = 256, inputHeight = 256;
  // 1. find max conf in small head: 32 * 32 * 6
  auto classData = outputs[0];
  int maxIndex = getMaxIndex(classData, 32 * 32 * 6);
  if (maxIndex == -1) {
    return;
  }
  result[7] = classData[maxIndex];
  // 2. which class
  int topk_cls = maxIndex % 6;
  result[6] = topk_cls;
  // 2.1 hand ceter point: (hand_x * 8, hand_y * 8)
  float hand_x = maxIndex / 6 % 32;
  float hand_y = maxIndex / 6 / 32;
  int top_ind = hand_y * 32 + hand_x;
  auto *wh = outputs[3];
  auto *hand_reg = outputs[4];
  hand_x = hand_x + hand_reg[2 * top_ind]; // offset
  hand_y = hand_y + hand_reg[2 * top_ind + 1]; // offset
  // 2.2 hand bbox
  float handBoxXMin = 8 * (hand_x - wh[2 * top_ind] / 2);
  float handBoxYMin = 8 * (hand_y - wh[2 * top_ind + 1] / 2);
  float handBoxXMax = 8 * (hand_x + wh[2 * top_ind] / 2);
  float handBoxYMax = 8 * (hand_y + wh[2 * top_ind + 1] / 2);
  // 3. one key point
  auto kp_hm = outputs[1];
  int topk_kp_maxIndex = getMaxIndex(kp_hm, 32 * 32);
  int topk_x = topk_kp_maxIndex % 32;
  int topk_y = std::floor(topk_kp_maxIndex / 32.0);
  // 4. key reg
  auto *kpReg = outputs[2];
  float regX = kpReg[topk_kp_maxIndex * 2];
  float regY = kpReg[topk_kp_maxIndex * 2 + 1];
  // 5. true kp x, y
  float fingerPointX = 8 * (topk_x + regX);
  float fingerPointY = 8 * (topk_y + regY);
  float scale = 1.0;
  float padSize = 0.0;
  if (originWidth > originHeight) {
    scale = originWidth / (float) inputWidth;
    padSize = (originWidth - originHeight) / 2.0;
    // handBoxRect
    result[0] = handBoxXMin * scale;
    result[1] = handBoxYMin * scale - padSize;
    result[2] = (handBoxXMax - handBoxXMin) * scale;
    result[3] = (handBoxYMax - handBoxYMin) * scale;
    // fingerPoint
    result[4] = fingerPointX * scale;
    result[5] = fingerPointY * scale - padSize;
  } else {
    scale = originHeight / (float) inputHeight;
    padSize = (originHeight - originWidth) / 2.0;
    // handBoxRect
    result[0] = handBoxXMin * scale - padSize;
    result[1] = handBoxYMin * scale;
    result[2] = (handBoxXMax - handBoxXMin) * scale;
    result[3] = (handBoxYMax - handBoxYMin) * scale;
    // fingerPoint
    result[4] = fingerPointX * scale - padSize;
    result[5] = fingerPointY * scale;
  }
}


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
) {
  jfloat *data_0 = env->GetFloatArrayElements(data0, NULL);
  jfloat *data_1 = env->GetFloatArrayElements(data1, NULL);
  jfloat *data_2 = env->GetFloatArrayElements(data2, NULL);
  jfloat *data_3 = env->GetFloatArrayElements(data3, NULL);
  jfloat *data_4 = env->GetFloatArrayElements(data4, NULL);
  float *outputs[5];
  outputs[0] = data_0;
  outputs[1] = data_1;
  outputs[2] = data_2;
  outputs[3] = data_3;
  outputs[4] = data_4;
  // jni返回的结果，0-3 表示 handBoxRect，4-5 表示 fingerPoint，6 表示 gestureLabel，7 表示 confidence
  float result[8];
  postprocess(result, outputs, imgWidth, imgHeight);

  jfloatArray resultArr = env->NewFloatArray(8);
  env->SetFloatArrayRegion(resultArr, 0, 8, result);

  env->ReleaseFloatArrayElements(data0, data_0, 0);
  env->ReleaseFloatArrayElements(data1, data_1, 0);
  env->ReleaseFloatArrayElements(data2, data_2, 0);
  env->ReleaseFloatArrayElements(data3, data_3, 0);
  env->ReleaseFloatArrayElements(data4, data_4, 0);
  return resultArr;
}

}

