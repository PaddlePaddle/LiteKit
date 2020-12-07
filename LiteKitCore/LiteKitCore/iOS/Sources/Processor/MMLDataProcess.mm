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


#import "MMLDataProcess.h"
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/core/core.hpp"

using namespace cv;

/**
 * 颜色空间转换枚举定义
*/
typedef NS_ENUM(int, TMMLMatColorSpace) {
    TMMLMatColorSpace_RGB2YCrCb = CV_RGB2YCrCb, //RGB -> YCrCb
    TMMLMatColorSpace_RGB2YUV   = CV_RGB2YUV,   //RGB -> YUV
};

@implementation MMLDataProcess

/**
  下采样
 
  @param src 待采样mat
  @param samplingRate 采样比例
 */
+ (Mat)mml_subSamplingCVMat:(Mat)src rate:(double)samplingRate {
    Mat pixel;
    
    if (samplingRate != 0) {
        src.convertTo(pixel, CV_32FC1, 1.f / samplingRate);
    }

    return pixel;
}

/**
  上采样
 
  @param src 待采样mat
  @param samplingRate 采样比例
 */
+ (Mat)mml_upSamplingCVMat:(Mat)src rate:(double)samplingRate {
    Mat pixel;
    src.convertTo(pixel, CV_8UC1, samplingRate);
    return pixel;
}


+ (UIImage *)mml_UIImageFromCVMat:(cv::Mat)image {
    NSData *data = [NSData dataWithBytes:image.data
                                  length:image.step.p[0] * image.rows];
    
    CGColorSpaceRef colorSpace;
    
    if (image.elemSize() == 1) {
        colorSpace = CGColorSpaceCreateDeviceGray();
    } else {
        colorSpace = CGColorSpaceCreateDeviceRGB();
    }
    
    CGDataProviderRef provider =
    CGDataProviderCreateWithCFData((__bridge CFDataRef)data);
    
    // Preserve alpha transparency, if exists
    BOOL alpha = (image.channels() == 4);
    CGBitmapInfo bitmapInfo = (alpha ? (kCGImageAlphaPremultipliedLast) : (kCGImageAlphaNone) | kCGBitmapByteOrderDefault);
    
    // Creating CGImage from cv::Mat
    CGImageRef imageRef = CGImageCreate(image.cols,
                                        image.rows,
                                        8 * image.elemSize1(), //The number of bits for each component in a source pixel. 
                                        8 * image.elemSize(),
                                        image.step.p[0],
                                        colorSpace,
                                        bitmapInfo,
                                        provider,
                                        NULL,
                                        false,
                                        kCGRenderingIntentDefault
                                        );
    
    
    // Getting UIImage from CGImage
    UIImage *finalImage = [UIImage imageWithCGImage:imageRef];
    CGImageRelease(imageRef);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(colorSpace);
    
    return finalImage;
}

#pragma mark -> warning 此方法copy自opencv的UIImageToMat, 解决opencv的方法丢掉了Alpha通道的问题, opencv修改后可使用opencv提供的方法
+ (cv::Mat)mml_CVMatFromUIImage:(UIImage *)image {
    cv::Mat m;
    CGColorSpaceRef colorSpace = CGImageGetColorSpace(image.CGImage);
    CGFloat cols = CGImageGetWidth(image.CGImage), rows = CGImageGetHeight(image.CGImage);
    CGContextRef contextRef;
    CGBitmapInfo bitmapInfo = kCGImageAlphaPremultipliedLast | kCGBitmapByteOrderDefault;
    if (CGColorSpaceGetModel(colorSpace) == kCGColorSpaceModelMonochrome) {
        m.create(rows, cols, CV_8UC1); // 8 bits per component, 1 channel
        bitmapInfo = kCGImageAlphaNone;
        contextRef = CGBitmapContextCreate(m.data, m.cols, m.rows, 8,
                                           m.step[0], colorSpace,
                                           bitmapInfo);
    } else {
        m.create(rows, cols, CV_8UC4); // 8 bits per component, 4 channels
        contextRef = CGBitmapContextCreate(m.data, m.cols, m.rows, 8,
                                           m.step[0], colorSpace,
                                           bitmapInfo);
    }
    CGContextDrawImage(contextRef, CGRectMake(0, 0, cols, rows), image.CGImage);
    CGContextRelease(contextRef);
    return  m;
}


+ (Mat)mml_CVMatNormalization:(Mat)src
              outputType:(TMMLMatDataType)outputType
                   alpha:(double)alphe
                    beta:(double)beta {
    Mat pixel;
    switch (outputType) {
        case TMMLMatDataType_32FC1:
            src.convertTo(pixel, CV_32FC1, alphe, beta);
            break;
            
        case TMMLMatDataType_8UC1:
            src.convertTo(pixel, CV_8UC1, alphe, beta);
            break;
            
        default:
            break;
    }
    
    return pixel;
}

+ (Mat)mml_scaleCVMat:(Mat)src
          toSize:(Size2i)size {
    Mat returnMat;
    resize(src, returnMat, size);
    return returnMat;
}

+ (Mat)mml_imageRGBCVMatFromYCrCb:(MMLImageData)ycrcbData {
    return [self mml_imageDataFromImageData:ycrcbData withColorSpace:CV_YCrCb2RGB];
}

+ (Mat)mml_imageRGBCVMatFromYUV:(MMLImageData)ycrcbData {
    return [self mml_imageDataFromImageData:ycrcbData withColorSpace:CV_YUV2RGB];
}

+ (Mat)mml_imageDataFromImageData:(MMLImageData)imageData
                   withColorSpace:(int)colorSpace {
    Mat returnMat;
    
    // merge
    NSInteger imageChannels = imageData.channels.size();
    
    if (imageChannels == 1) {
        imageData.channels.at(0).copyTo(returnMat);
    } else if (imageChannels >= 3) {
        Mat channels;
        merge(imageData.channels, channels);
        
        Mat RGB;
        cvtColor(channels, RGB, colorSpace, 0);
        
        if (!imageData.alpha.empty()) {
            Mat RGBA;
            cvtColor(RGB, RGBA, CV_RGB2RGBA);
            insertChannel(imageData.alpha, RGBA, 3);
            RGB = RGBA;
        }
        
        returnMat = RGB;
    }
    
    return returnMat;
}

+ (MMLImageData)mml_YCrCbFromImageRGBCVMat:(Mat)imageMat {
    return [self matsFromImageData:imageMat withColorSpace:TMMLMatColorSpace_RGB2YCrCb];
}
+ (MMLImageData)mml_YUVFromImageRGBCVMat:(Mat)imageMat {
    return [self matsFromImageData:imageMat withColorSpace:TMMLMatColorSpace_RGB2YUV];
}

+ (MMLImageData)matsFromImageData:(Mat)imageMat
                   withColorSpace:(TMMLMatColorSpace)colorSpace  {
    // split
    std::vector<Mat> channels;
    Mat alpha;
    
    if (imageMat.channels() == 1) {
        channels.push_back(imageMat);
    } else if (imageMat.channels() >= 3) {
        Mat YCbCr;
        cvtColor(imageMat, YCbCr, colorSpace, 0);
        split(YCbCr, channels);
        
        if (imageMat.channels() == 4) {
            extractChannel(imageMat, alpha, 3);
        }
    }
    
    MMLImageData returnData;
    returnData.channels = channels;
    returnData.alpha = alpha;
    
    return returnData;
}

/**
 Mat -> Point *转换

 @param src Mat数据
 @param dst Point *数据,待填充
*/
template<typename T> void mml_convertCVMatToData(Mat src, T * _Nullable dst) {
    BOOL continuous = NO;
    if (src.isContinuous()) {
        continuous = YES;
    }
    
    // input data
    int w = src.cols;
    int h = src.rows;
    
    // input data
    if (continuous) {
        memcpy(dst, src.data, w * h * sizeof(T));
    } else {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                dst[i * w + j] = src.at<T>(i, j);
            }
        }
    }
    return;
}

/**
 Point * -> Mat转换
 
 @param src Point *数据
 @param dst Mat数据，待填充
*/
template<typename T> void mml_convertDataToCVMat(T * _Nullable src,  Mat dst) {
    BOOL continuous = NO;
    if (dst.isContinuous()) {
        continuous = YES;
    }
    int w = dst.cols;
    int h = dst.rows;

    if (continuous) {
        memcpy(dst.data, src, w * h * sizeof(T));
    } else {
        for (int i = 0; i < h; ++i) {
           for (int j = 0; j < w; ++j) {
               dst.at<T>(i, j) = src[i * w + j];
           }
       }
    }
    return;
}

/**
  提取mat数据
 
  @param inputMat inputMat
 */
+ (void)mml_convertCVMatData:(Mat)inputMat toFloatData:(float *)image_data {
    mml_convertCVMatToData<float>(inputMat, image_data);
}

+ (void)mml_convertFloatData:(float *)image_data toCVMatData:(Mat)inputMat {
    mml_convertDataToCVMat<float>(image_data, inputMat);
}

+ (void)mml_convertCVMatData:(Mat)inputMat toUint8Data:(uint8_t *)image_data {
    mml_convertCVMatToData<uint8_t>(inputMat, image_data);
}

+ (void)mml_convertUint8Data:(uint8_t *)image_data toCVMatData:(Mat)inputMat {
    mml_convertDataToCVMat<uint8_t>(image_data, inputMat);
}

+ (Mat)mml_shearCVMat:(Mat)inputMat
                 rect:(Rect2i)rect {
    Mat returnMat = inputMat(rect);
    return returnMat;
}


+ (Mat)mml_borderCVMat:(Mat)inputMat
                margin:(MMLMargin)margin
                  type:(TMMLBorderTypes)borderType {
    if ( 0 == margin.top && 0 == margin.bottom && 0 == margin.left && 0 == margin.right ) {
        return inputMat;
    }
    
    Mat outputMat;
    copyMakeBorder(inputMat, outputMat,
                   margin.top, margin.bottom, margin.left, margin.right,
                   borderType);
    return outputMat;
}

@end
