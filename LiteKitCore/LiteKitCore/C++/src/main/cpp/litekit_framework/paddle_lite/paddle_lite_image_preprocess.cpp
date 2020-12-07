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

#include "paddle_lite_image_preprocess.h"
#include "../paddle_lite_header/paddle_image_preprocess.h"
#include "../common_log.h"

using namespace mml_framework;
using namespace lite;

paddle::lite::utils::cv::ImageFormat srcFormatFromMMLFormat(ImageFormat srcFormat) {
    paddle::lite::utils::cv::ImageFormat returnValue;
    switch (srcFormat) {
        case RGBA :{
            returnValue = paddle::lite::utils::cv::ImageFormat::RGBA;
            break;
        }
        case BGRA :{
            returnValue = paddle::lite::utils::cv::ImageFormat::BGRA;
            break;
        }
        case RGB :{
            returnValue = paddle::lite::utils::cv::ImageFormat::RGB;
            break;
        }
        case BGR :{
            returnValue = paddle::lite::utils::cv::ImageFormat::BGR;
            break;
        }
        case GRAY :{
            returnValue = paddle::lite::utils::cv::ImageFormat::GRAY;
            break;
        }
        case NV21 :{
            returnValue = paddle::lite::utils::cv::ImageFormat::NV21;
            break;
        }
        case NV12 :{
            returnValue = paddle::lite::utils::cv::ImageFormat::NV12;
            break;
        }
            
        default: {
            break;
        }
    }
    
    return returnValue;
}

paddle::lite::utils::cv::FlipParam filpParamFromMMLFlipParam(FlipParam flip_param) {
    paddle::lite::utils::cv::FlipParam returnValue;
    
    switch (flip_param) {
        case XY: {
            returnValue = paddle::lite::utils::cv::FlipParam::XY;
            break;
        }
        case X: {
            returnValue = paddle::lite::utils::cv::FlipParam::X;
            break;
        }
        case Y: {
            returnValue = paddle::lite::utils::cv::FlipParam::Y;
            break;
        }
              
        default:
            break;
    }
    
    return returnValue;
}

/*
* init
* param srcFormat: input image color
* param dstFormat: output image color
* param param: input image parameter, egs: input size
*/
PaddleLiteImagePreprocess::PaddleLiteImagePreprocess(ImageFormat srcFormat,
                                                     ImageFormat dstFormat,
                                                     TransParam param) {
    paddle::lite::utils::cv::ImageFormat srcFormatLite = srcFormatFromMMLFormat(srcFormat);
    paddle::lite::utils::cv::ImageFormat dstFormatLite = srcFormatFromMMLFormat(dstFormat);
    paddle::lite::utils::cv::TransParam paramLite = paddle::lite::utils::cv::TransParam{
        .ih = param.ih,                // input height
        .iw = param.iw,                // input width
        .oh = param.oh,                // outpu theight
        .ow = param.ow,                // output width
        .flip_param = filpParamFromMMLFlipParam(param.flip_param),  // flip, support x, y, xy
        .rotate_param = param.rotate_param
    };
    
    this->preprocesser = new paddle::lite::utils::cv::ImagePreprocess(srcFormatLite, dstFormatLite, paramLite);
}

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
void PaddleLiteImagePreprocess::imageConvert(const uint8_t* src, uint8_t* dst) {
    paddle::lite::utils::cv::ImagePreprocess *preprocesser = (paddle::lite::utils::cv::ImagePreprocess *)this->preprocesser;
    preprocesser->imageConvert(src, dst);
}
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
void PaddleLiteImagePreprocess::imageConvert(const uint8_t* src,
                                             uint8_t* dst,
                                             ImageFormat srcFormat,
                                             ImageFormat dstFormat) {
    paddle::lite::utils::cv::ImageFormat srcFormatLite = srcFormatFromMMLFormat(srcFormat);
    paddle::lite::utils::cv::ImageFormat dstFormatLite = srcFormatFromMMLFormat(dstFormat);
    
    paddle::lite::utils::cv::ImagePreprocess *preprocesser = (paddle::lite::utils::cv::ImagePreprocess *)this->preprocesser;
    preprocesser->imageConvert(src, dst, srcFormatLite, dstFormatLite);
}
/*
* image resize, use bilinear method
* support image format: 1-channel image (egs: GRAY, 2-channel image (egs:
* NV12, NV21), 3-channel(egs: BGR), 4-channel(egs: BGRA)
* param src: input image data
* param dst: output image data
*/
void PaddleLiteImagePreprocess::imageResize(const uint8_t* src, uint8_t* dst) {
    paddle::lite::utils::cv::ImagePreprocess *preprocesser = (paddle::lite::utils::cv::ImagePreprocess *)this->preprocesser;
    preprocesser->imageResize(src, dst);
}
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
void PaddleLiteImagePreprocess::imageResize(const uint8_t* src,
                                            uint8_t* dst,
                                            ImageFormat srcFormat,
                                            int srcw,
                                            int srch,
                                            int dstw,
                                            int dsth){
    paddle::lite::utils::cv::ImagePreprocess *preprocesser = (paddle::lite::utils::cv::ImagePreprocess *)this->preprocesser;
    
    paddle::lite::utils::cv::ImageFormat srcFormatLite = srcFormatFromMMLFormat(srcFormat);
    
    preprocesser->imageResize(src, dst, srcFormatLite, srcw, srch, dstw, dsth);
}

/*
* image Rotate
* support 90, 180 and 270 Rotate process
* color format support 1-channel image, 3-channel image and 4-channel image
* param src: input image data
* param dst: output image data
*/
void PaddleLiteImagePreprocess::imageRotate(const uint8_t* src, uint8_t* dst) {
    paddle::lite::utils::cv::ImagePreprocess *preprocesser = (paddle::lite::utils::cv::ImagePreprocess *)this->preprocesser;
    preprocesser->imageRotate(src, dst);
}
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
void PaddleLiteImagePreprocess::imageRotate(const uint8_t* src,
                                            uint8_t* dst,
                                            ImageFormat srcFormat,
                                            int srcw,
                                            int srch,
                                            float degree) {
    paddle::lite::utils::cv::ImagePreprocess *preprocesser = (paddle::lite::utils::cv::ImagePreprocess *)this->preprocesser;
    paddle::lite::utils::cv::ImageFormat srcFormatLite = srcFormatFromMMLFormat(srcFormat);
    preprocesser->imageRotate(src, dst, srcFormatLite, srcw, srch, degree);
}
/*
* image Flip
* support X, Y and XY flip process
* color format support 1-channel image, 3-channel image and 4-channel image
* param src: input image data
* param dst: output image data
*/
void PaddleLiteImagePreprocess::imageFlip(const uint8_t* src, uint8_t* dst) {
    paddle::lite::utils::cv::ImagePreprocess *preprocesser = (paddle::lite::utils::cv::ImagePreprocess *)this->preprocesser;
    preprocesser->imageFlip(src, dst);
}
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
void PaddleLiteImagePreprocess::imageFlip(const uint8_t* src,
                                          uint8_t* dst,
                                          ImageFormat srcFormat,
                                          int srcw,
                                          int srch,
                                          FlipParam flip_param) {
    paddle::lite::utils::cv::ImagePreprocess *preprocesser = (paddle::lite::utils::cv::ImagePreprocess *)this->preprocesser;
    paddle::lite::utils::cv::ImageFormat srcFormatLite = srcFormatFromMMLFormat(srcFormat);
    paddle::lite::utils::cv::FlipParam flip_param_lite = filpParamFromMMLFlipParam(flip_param);
    
    preprocesser->imageFlip(src, dst, srcFormatLite, srcw, srch, flip_param_lite);
}


paddle::lite::utils::cv::LayoutType layoutTypeFromMMLLayoutType(DataLayoutType layout) {
    paddle::lite::utils::cv::LayoutType returnValue;
    switch (layout) {
        case kUnk :{
            returnValue =  paddle::lite::utils::cv::LayoutType::kUnk;
            break;
        }
        case kNCHW:{
            returnValue =  paddle::lite::utils::cv::LayoutType::kNCHW;
            break;
        }
        case kNHWC:{
            returnValue =  paddle::lite::utils::cv::LayoutType::kNHWC;
            break;
        }
        case kImageDefault:{// for opencl image2d
            returnValue =  paddle::lite::utils::cv::LayoutType::kImageDefault;
            break;
        }
        case kImageFolder:{// for opencl image2d
            returnValue =  paddle::lite::utils::cv::LayoutType::kImageFolder;
            break;
        }
        case kImageNW:{// for opencl image2d
            returnValue =  paddle::lite::utils::cv::LayoutType::kImageNW;
            break;
        }
        case kAny:{ // any data layout
            returnValue =  paddle::lite::utils::cv::LayoutType::kAny;
            break;
        }
        case NUM:{// number of fields.
            returnValue =  paddle::lite::utils::cv::LayoutType::NUM;
            break;
        }
        default:
            break;
    }
    return returnValue;
}
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
void PaddleLiteImagePreprocess::image2Tensor(const uint8_t* src,
                                             Tensor* dstTensor,
                                             DataLayoutType layout,
                                             float* means,
                                             float* scales) {
    paddle::lite::utils::cv::ImagePreprocess *preprocesser = (paddle::lite::utils::cv::ImagePreprocess *)this->preprocesser;
    paddle::lite::utils::cv::LayoutType layoutLite = layoutTypeFromMMLLayoutType(layout);
    preprocesser->image2Tensor(src, (paddle::lite_api::Tensor *)dstTensor->tensor, layoutLite, means, scales);
}
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
void PaddleLiteImagePreprocess::image2Tensor(const uint8_t* src,
                                             Tensor* dstTensor,
                                             ImageFormat srcFormat,
                                             int srcw,
                                             int srch,
                                             DataLayoutType layout,
                                             float* means,
                                             float* scales) {
    paddle::lite::utils::cv::ImagePreprocess *preprocesser = (paddle::lite::utils::cv::ImagePreprocess *)this->preprocesser;
    paddle::lite::utils::cv::LayoutType layoutLite = layoutTypeFromMMLLayoutType(layout);
    paddle::lite::utils::cv::ImageFormat srcFormatLite = srcFormatFromMMLFormat(srcFormat);
    
    preprocesser->image2Tensor(src, (paddle::lite_api::Tensor *)dstTensor->tensor, srcFormatLite, srcw, srch, layoutLite, means, scales);
}

PaddleLiteImagePreprocess::~PaddleLiteImagePreprocess() {
    LOGI("destructor of PaddleLiteImagePreprocess");
    if (preprocesser != nullptr) {
        delete preprocesser;
        preprocesser = nullptr;
    }
}
