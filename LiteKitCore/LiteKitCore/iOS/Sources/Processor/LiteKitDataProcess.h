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


#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import <opencv2/core/utility.hpp>
#import <opencv2/imgproc.hpp>
#import <opencv2/core/core.hpp>

NS_ASSUME_NONNULL_BEGIN

using namespace cv;
@class LiteKitData;

/**
 * LiteKit image data struct
 */
typedef struct {
    std::vector<cv::Mat> channels;//color channel after split
    Mat alpha;//Alpha
} LiteKitImageData;

/**
 * LiteKitnormalization中，return data type enum
*/
typedef NS_ENUM(NSUInteger, TLiteKitMatDataType) {
    TLiteKitMatDataType_32FC1,//return mat as 32f c1 format
    TLiteKitMatDataType_8UC1,//return mat as 8u c1 format
};

#pragma mark fill Mat &struct
/**
 * LiteKit margin data struct，used in border
*/
typedef struct {
    int top;
    int bottom;
    int left;
    int right;
} LiteKitMargin;

/**
 * LiteKitborder fill type
*/
typedef NS_ENUM(int,  TLiteKitBorderTypes) {
    TLiteKitBorderConstant    = BORDER_CONSTANT, //!< `iiiiii|abcdefgh|iiiiiii`  with some specified `i`
    TLiteKitBorderReplicate   = BORDER_REPLICATE, //!< `aaaaaa|abcdefgh|hhhhhhh`
    TLiteKitBorderReflect     = BORDER_REFLECT, //!< `fedcba|abcdefgh|hgfedcb`
    TLiteKitBorderWarp        = BORDER_WRAP, //!< `cdefgh|abcdefgh|abcdefg`
    TLiteKitBorderReflect_101 = BORDER_REFLECT_101, //!< `gfedcb|abcdefgh|gfedcba`
    TLiteKitBorderTransparent = BORDER_TRANSPARENT, //!< `uvwxyz|abcdefgh|ijklmno`
    TLiteKitBorderIsolated    = BORDER_ISOLATED, //!< do not look outside of ROI
    TLiteKitBorderDefault     = BORDER_DEFAULT, //!< same as BORDER_REFLECT_101
};

#pragma mark class define
/**
 * LiteKit data process tool
*/
@interface LiteKitDataProcess : NSObject

/**
  up sampling
 
  @param src input mat
  @param samplingRate sampling rate
  @return Mat after sampling，if samplingRate=0，invalid，will return empty mat
 */
+ (Mat)litekit_subSamplingCVMat:(Mat)src rate:(double)samplingRate;
/**
 down sampling

 @param src inputmat
 @param samplingRate sampling rate
 @return Mat after sampling
*/
+ (Mat)litekit_upSamplingCVMat:(Mat)src rate:(double)samplingRate;

/**
 UIimage -> Mat convert

 @param src input image data
 @return converted UIimage data
*/
+ (UIImage *)litekit_UIImageFromCVMat:(Mat)src;

/**
 Mat -> UIimage convert

 @param src input image data
 @return converted Mat data
*/
+ (Mat)litekit_CVMatFromUIImage:(UIImage *)src;


/**
 normalization

 @param src input image data
 @param outputType retturn Mat data type
 @param alpha coefficient，return = src * alpha + beta
 @param beta shifting，return = src * alpha + beta
 @return Mat converted data。if outputType not support，will return empty mat
*/
+ (Mat)litekit_CVMatNormalization:(Mat)src
                   outputType:(TLiteKitMatDataType)outputType
                        alpha:(double)alpha
                         beta:(double)beta;

/**
 Mat Resize

 @param src  input image data
 @param size output size
 @return Mat scaled size
*/
+ (Mat)litekit_scaleCVMat:(Mat)src toSize:(Size2i)size;

/**
 RGB LiteKit image data struct convert to Mat

 @param ycrcbData YCrCb data
 @return Mat converted mat，if ycrcbData channels != 1、3、4 will not support，will return empty mat
*/
+ (Mat)litekit_imageRGBCVMatFromYCrCb:(LiteKitImageData)ycrcbData;

/**
 RGB LiteKit image data struct convert to Mat

 @param yuvData YUV data
 @return Mat converted mat，if ycrcbData channels != 1、3、4 will not suppoort，will return empty mat
*/
+ (Mat)litekit_imageRGBCVMatFromYUV:(LiteKitImageData)yuvData;

/**
 Mat to LiteKit image data

 @param imageMat YCrCb data
 @return LiteKitImageData RGB LiteKit image data struct，if imageMat channels != 1、3、4will not support，will return empty mat
*/
+ (LiteKitImageData)litekit_YCrCbFromImageRGBCVMat:(Mat)imageMat;

/**
 Mat转LiteKit image data struct

 @param imageMat YUV data
 @return LiteKitImageData RGB LiteKit image data struct，if imageMat channels != 1、3、4will not support，will return empty mat
*/
+ (LiteKitImageData)litekit_YUVFromImageRGBCVMat:(Mat)imageMat;

/**
 Mat -> float * converter

 @param inputMat Mat data
 @param image_data float* data, to be writed
*/
+ (void)litekit_convertCVMatData:(Mat)inputMat toFloatData:(float *)image_data;///Mat    >> float*

/**
 float * -> Mat converter

 @param image_data float* data
 @param inputMat Mat data，to be writed
*/
+ (void)litekit_convertFloatData:(float *)image_data toCVMatData:(Mat)inputMat;///float* >> Mat

/**
 Mat -> uint8_t * converter

 @param inputMat Mat data
 @param image_data uint8_t* data,to be writed
*/
+ (void)litekit_convertCVMatData:(Mat)inputMat toUint8Data:(uint8_t *)image_data;

/**
 uint8_t * -> Mat converter
 
 @param image_data uint8_t* data
 @param inputMat Mat data，to be writed
*/
+ (void)litekit_convertUint8Data:(uint8_t *)image_data toCVMatData:(Mat)inputMat;

/**
 shear Mat

 @param inputMat Mat data
 @param rect shear rect
 @return Mat sheared Mat
*/
+ (Mat)litekit_shearCVMat:(Mat)inputMat rect:(Rect2i)rect;

/**
 border Mat

 @param inputMat Mat data
 @param margin border margin
 @param borderTypre border type
 @return Mat border Mat
*/
+ (Mat)litekit_borderCVMat:(Mat)inputMat margin:(LiteKitMargin)margin type:( TLiteKitBorderTypes)borderTypre;


@end

NS_ASSUME_NONNULL_END
