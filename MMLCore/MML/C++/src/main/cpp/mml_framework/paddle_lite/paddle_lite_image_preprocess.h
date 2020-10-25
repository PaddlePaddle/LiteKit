//
//  paddle_lite_image_preprocess.h
//  MMLNativeDemo
//
//  Created by Baidu Co.Ltd. on 2020/3/2.
//  Copyright © 2020 Baidu Co.Ltd. All rights reserved.
//

#ifndef paddle_lite_image_preprocess_h
#define paddle_lite_image_preprocess_h

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include "../mml_inference_api.h"

namespace mml_framework {
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

typedef mml_framework::MMLTensor Tensor;

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

}  // namespace mml_framework
}  // namespace lite

#endif /* paddle_lite_image_preprocess_h */
