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
Java_com_baidu_litekitcore_demo_MainActivity_preprocess(JNIEnv *env,
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
