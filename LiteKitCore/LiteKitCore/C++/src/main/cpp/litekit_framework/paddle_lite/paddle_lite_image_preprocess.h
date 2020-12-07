/**
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

//
//  paddle_lite_image_preprocess.h
//  LiteKitNativeDemo
//
//  Created by Baidu Co.Ltd. on 2020/3/2.
//  Copyright © 2020 Baidu Co.Ltd. All rights reserved.
//

#ifndef paddle_lite_image_preprocess_h
#define paddle_lite_image_preprocess_h

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include "../litekit_inference_api.h"

namespace litekit_framework {
namespace lite {

enum DataLayoutType {
  kUnk = 0,
  kNCHW = 1,
  kNHWC = 3,
  kImageDefault = 4,  // for opencl image2d
  kImageFolder = 5,   // for opencl image2d
  kImageNW = 6,       // for opencl image2d
  kAny = 2,           // any data layout
  NUM = 7,            // number of fields.
};

// color enum
enum ImageFormat {
  RGBA = 0,
  BGRA,
  RGB,
  BGR,
  GRAY,
  NV21 = 11,
  NV12,
};
// flip enum
enum FlipParam {
  XY = -1,  // flip along the XY axis
  X = 0,    // flip along the X axis
  Y         // flip along the Y axis
};
// transform param
typedef struct {
  int ih;                // input height
  int iw;                // input width
  int oh;                // outpu theight
  int ow;                // output width
  FlipParam flip_param;  // flip, support x, y, xy
  float rotate_param;    // rotate, support 90, 180, 270
} TransParam;

typedef litekit_framework::LiteKitTensor Tensor;

class PaddleLiteImagePreprocess {
 public:
  /*
  * init
  * param srcFormat: input image color
  * param dstFormat: output image color
  * param param: input image parameter, egs: input size
  */
  PaddleLiteImagePreprocess(ImageFormat srcFormat,
                            ImageFormat dstFormat,
                            TransParam param);

  /*
  * image color convert
  * support NV12/NV21_to_BGR(RGB), NV12/NV21_to_BGRA(RGBA),
  * BGR(RGB)and BGRA(RGBA) transform,
  * BGR(RGB)and RGB(BGR) transform,
  * BGR(RGB)and RGBA(BGRA) transform,
  * BGR(RGB) and GRAY transform,
  * BGRA(RGBA) and GRAY transform,
  * param src: input image data
  * param dst: output image data
  */
  void imageConvert(const uint8_t* src, uint8_t* dst);
  /*
  * image color convert
  * support NV12/NV21_to_BGR(RGB), NV12/NV21_to_BGRA(RGBA),
  * BGR(RGB)and BGRA(RGBA) transform,
  * BGR(RGB)and RGB(BGR) transform,
  * BGR(RGB)and RGBA(BGRA) transform,
  * BGR(RGB)and GRAY transform,
  * BGRA(RGBA) and GRAY transform,
  * param src: input image data
  * param dst: output image data
  * param srcFormat: input image image format support: GRAY, NV12(NV21),
  * BGR(RGB) and BGRA(RGBA)
  * param dstFormat: output image image format, support GRAY, BGR(RGB) and
  * BGRA(RGBA)
  */
  void imageConvert(const uint8_t* src,
                    uint8_t* dst,
                    ImageFormat srcFormat,
                    ImageFormat dstFormat);
  /*
  * image resize, use bilinear method
  * support image format: 1-channel image (egs: GRAY, 2-channel image (egs:
  * NV12, NV21), 3-channel(egs: BGR), 4-channel(egs: BGRA)
  * param src: input image data
  * param dst: output image data
  */
  void imageResize(const uint8_t* src, uint8_t* dst);
  /*
   image resize, use bilinear method
  * support image format: 1-channel image (egs: GRAY, 2-channel image (egs:
  NV12, NV21), 3-channel image(egs: BGR), 4-channel image(egs: BGRA)
  * param src: input image data
  * param dst: output image data
  * param srcw: input image width
  * param srch: input image height
  * param dstw: output image width
  * param dsth: output image height
  */
  void imageResize(const uint8_t* src,
                   uint8_t* dst,
                   ImageFormat srcFormat,
                   int srcw,
                   int srch,
                   int dstw,
                   int dsth);

  /*
  * image Rotate
  * support 90, 180 and 270 Rotate process
  * color format support 1-channel image, 3-channel image and 4-channel image
  * param src: input image data
  * param dst: output image data
  */
  void imageRotate(const uint8_t* src, uint8_t* dst);
  /*
  * image Rotate
  * support 90, 180 and 270 Rotate process
  * color format support 1-channel image, 3-channel image and 4-channel image
  * param src: input image data
  * param dst: output image data
  * param srcFormat: input image format, support GRAY, BGR(RGB) and BGRA(RGBA)
  * param srcw: input image width
  * param srch: input image height
  * param degree: Rotate degree, support 90, 180 and 270
  */
  void imageRotate(const uint8_t* src,
                   uint8_t* dst,
                   ImageFormat srcFormat,
                   int srcw,
                   int srch,
                   float degree);
  /*
  * image Flip
  * support X, Y and XY flip process
  * color format support 1-channel image, 3-channel image and 4-channel image
  * param src: input image data
  * param dst: output image data
  */
  void imageFlip(const uint8_t* src, uint8_t* dst);
  /*
  * image Flip
  * support X, Y and XY flip process
  * color format support 1-channel image, 3-channel image and 4-channel image
  * param src: input image data
  * param dst: output image data
  * param srcFormat: input image format, support GRAY, BGR(RGB) and BGRA(RGBA)
  * param srcw: input image width
  * param srch: input image height
  * param flip_param: flip parameter, support X, Y and XY
  */
  void imageFlip(const uint8_t* src,
                 uint8_t* dst,
                 ImageFormat srcFormat,
                 int srcw,
                 int srch,
                 FlipParam flip_param);
  /*
  * change image data to tensor data
  * support image format is GRAY, BGR(RGB) and BGRA(RGBA), Data layout is NHWC
  * and
  * NCHW
  * param src: input image data
  * param dstTensor: output tensor data
  * param layout: output tensor layout，support NHWC and NCHW
  * param means: means of image
  * param scales: scales of image
  */
  void image2Tensor(const uint8_t* src,
                    Tensor* dstTensor,
                    DataLayoutType layout,
                    float* means,
                    float* scales);
  /*
   * change image data to tensor data
  * support image format is GRAY, BGR(RGB) and BGRA(RGBA), Data layout is NHWC
  * and
  * NCHW
  * param src: input image data
  * param dstTensor: output tensor data
  * param srcFormat: input image format, support BGR(RGB) and BGRA(RGBA)
  * param srcw: input image width
  * param srch: input image height
  * param layout: output tensor layout，support NHWC and NCHW
  * param means: means of image
  * param scales: scales of image
  */
  void image2Tensor(const uint8_t* src,
                    Tensor* dstTensor,
                    ImageFormat srcFormat,
                    int srcw,
                    int srch,
                    DataLayoutType layout,
                    float* means,
                    float* scales);

    /**
     * 析构函数
     */
    ~PaddleLiteImagePreprocess();
    
 private:
    void *preprocesser;
    ImageFormat srcFormat_;
    ImageFormat dstFormat_;
    TransParam transParam_;
};

}  // namespace litekit_framework
}  // namespace lite

#endif /* paddle_lite_image_preprocess_h */
