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


#import "LiteKitDataProcess.h"
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/core/core.hpp"

using namespace cv;

/**
 * color space enum
*/
typedef NS_ENUM(int, TLiteKitMatColorSpace) {
    TLiteKitMatColorSpace_RGB2YCrCb = CV_RGB2YCrCb, //RGB -> YCrCb
    TLiteKitMatColorSpace_RGB2YUV   = CV_RGB2YUV,   //RGB -> YUV
};

@implementation LiteKitDataProcess

/**
  downsampling
 
  @param src input mat
  @param samplingRate sampling rate
 */
+ (Mat)litekit_subSamplingCVMat:(Mat)src rate:(double)samplingRate {
    Mat pixel;
    
    if (samplingRate != 0) {
        src.convertTo(pixel, CV_32FC1, 1.f / samplingRate);
    }

    return pixel;
}

/**
  upsampling
 
  @param src input mat
  @param samplingRate upsampling rate
 */
+ (Mat)litekit_upSamplingCVMat:(Mat)src rate:(double)samplingRate {
    Mat pixel;
    src.convertTo(pixel, CV_8UC1, samplingRate);
    return pixel;
}


+ (UIImage *)litekit_UIImageFromCVMat:(cv::Mat)image {
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

#pragma mark -> warning this function copy from opencv/UIImageToMat, to resolve opencv method throw Alpha channel, use opencv function after opencv fixed
+ (cv::Mat)litekit_CVMatFromUIImage:(UIImage *)image {
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


+ (Mat)litekit_CVMatNormalization:(Mat)src
              outputType:(TLiteKitMatDataType)outputType
                   alpha:(double)alphe
                    beta:(double)beta {
    Mat pixel;
    switch (outputType) {
        case TLiteKitMatDataType_32FC1:
            src.convertTo(pixel, CV_32FC1, alphe, beta);
            break;
            
        case TLiteKitMatDataType_8UC1:
            src.convertTo(pixel, CV_8UC1, alphe, beta);
            break;
            
        default:
            break;
    }
    
    return pixel;
}

+ (Mat)litekit_scaleCVMat:(Mat)src
          toSize:(Size2i)size {
    Mat returnMat;
    resize(src, returnMat, size);
    return returnMat;
}

+ (Mat)litekit_imageRGBCVMatFromYCrCb:(LiteKitImageData)ycrcbData {
    return [self litekit_imageDataFromImageData:ycrcbData withColorSpace:CV_YCrCb2RGB];
}

+ (Mat)litekit_imageRGBCVMatFromYUV:(LiteKitImageData)ycrcbData {
    return [self litekit_imageDataFromImageData:ycrcbData withColorSpace:CV_YUV2RGB];
}

+ (Mat)litekit_imageDataFromImageData:(LiteKitImageData)imageData
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

+ (LiteKitImageData)litekit_YCrCbFromImageRGBCVMat:(Mat)imageMat {
    return [self matsFromImageData:imageMat withColorSpace:TLiteKitMatColorSpace_RGB2YCrCb];
}
+ (LiteKitImageData)litekit_YUVFromImageRGBCVMat:(Mat)imageMat {
    return [self matsFromImageData:imageMat withColorSpace:TLiteKitMatColorSpace_RGB2YUV];
}

+ (LiteKitImageData)matsFromImageData:(Mat)imageMat
                   withColorSpace:(TLiteKitMatColorSpace)colorSpace  {
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
    
    LiteKitImageData returnData;
    returnData.channels = channels;
    returnData.alpha = alpha;
    
    return returnData;
}

/**
 Mat -> Point * convert

 @param src Mat data
 @param dst Point * data,to be write
*/
template<typename T> void litekit_convertCVMatToData(Mat src, T * _Nullable dst) {
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
 Point * -> Mat convert
 
 @param src Point *data
 @param dst Mat data，to be write
*/
template<typename T> void litekit_convertDataToCVMat(T * _Nullable src,  Mat dst) {
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
  get data from mat
 
  @param inputMat inputMat
 */
+ (void)litekit_convertCVMatData:(Mat)inputMat toFloatData:(float *)image_data {
    litekit_convertCVMatToData<float>(inputMat, image_data);
}

+ (void)litekit_convertFloatData:(float *)image_data toCVMatData:(Mat)inputMat {
    litekit_convertDataToCVMat<float>(image_data, inputMat);
}

+ (void)litekit_convertCVMatData:(Mat)inputMat toUint8Data:(uint8_t *)image_data {
    litekit_convertCVMatToData<uint8_t>(inputMat, image_data);
}

+ (void)litekit_convertUint8Data:(uint8_t *)image_data toCVMatData:(Mat)inputMat {
    litekit_convertDataToCVMat<uint8_t>(image_data, inputMat);
}

+ (Mat)litekit_shearCVMat:(Mat)inputMat
                 rect:(Rect2i)rect {
    Mat returnMat = inputMat(rect);
    return returnMat;
}


+ (Mat)litekit_borderCVMat:(Mat)inputMat
                margin:(LiteKitMargin)margin
                  type:( TLiteKitBorderTypes)borderType {
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
