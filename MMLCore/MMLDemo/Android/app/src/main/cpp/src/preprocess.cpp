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
#include "preprocess.h"
#include "common_log.h"
#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

namespace imba {

static const int inputBatch = 1;
static const int inputChannel = 3;
static const int inputHeight = 256;
static const int inputWidth = 256;


void preprocess(cv::Mat src, cv::Mat dst) {
  const int w = src.cols;
  const int h = src.rows;
  const int padLength = abs(w - h) / 2;
  cv::Mat padImage;
  if (w < h) {
    cv::copyMakeBorder(src,
                       padImage,
                       0,
                       0,
                       padLength,
                       padLength,
                       cv::BORDER_CONSTANT,
                       cvScalar(255, 0, 0));
  } else {
    cv::copyMakeBorder(src,
                       padImage,
                       padLength,
                       padLength,
                       0,
                       0,
                       cv::BORDER_CONSTANT,
                       cvScalar(255, 0, 0));
  }
  cv::resize(padImage, dst, dst.size());
}

extern "C"
JNIEXPORT jfloatArray JNICALL
Java_com_baidu_mml_demo_MainActivity_preprocess(JNIEnv *env,
                                               jobject thiz,
                                               jbyteArray data,
                                               jint imgWidth,
                                               int imgHeight) {
  jbyte *data_ptr = env->GetByteArrayElements(data, NULL);
  if (data_ptr == NULL) {
    return nullptr;
  }
  jint data_len = env->GetArrayLength(data);
  LOGD("handGestureDetect data_len = %d", data_len);
  if (data_len == 0 || data_len != (imgWidth * imgHeight * 4)) {
    return 0;
  }
  cv::Mat imgMat(imgHeight, imgWidth, CV_8UC4, data_ptr);
  LOGD("preprocess start");
  cv::Mat rgbaMat(inputHeight, inputWidth, CV_8UC4);
  preprocess(imgMat, rgbaMat);

  ino64_t size = inputChannel * inputBatch * inputWidth * inputHeight;
  float *inputData = new float[size];
  float r, g, b;
  int ch_step = inputHeight * inputWidth;
  cv::Vec4b rgbaValue;
  for (int y = 0; y < inputHeight; y++) {
    for (int x = 0; x < inputWidth; x++) {
      rgbaValue = rgbaMat.at<cv::Vec4b>(y, x);
      r = (float) rgbaValue[0];
      g = (float) rgbaValue[1];
      b = (float) rgbaValue[2];
      int index = y * inputWidth + x;
      inputData[index] = r;
      inputData[ch_step + index] = g;
      inputData[2 * ch_step + index] = b;
    }
  }
  LOGD("preprocess end");
  jfloatArray resultArr = env->NewFloatArray(size);
  env->SetFloatArrayRegion(resultArr, 0, size, inputData);
  delete[] inputData;
  return resultArr;
}

}
