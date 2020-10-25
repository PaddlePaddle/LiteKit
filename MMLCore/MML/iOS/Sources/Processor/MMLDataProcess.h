//
//  MMLDataProcess.h
//  MML+Plugin
//
//  Created by Baidu. Co.,Ltd. on 2019/11/14.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import <opencv2/core/utility.hpp>
#import <opencv2/imgproc.hpp>
#import <opencv2/core/core.hpp>

NS_ASSUME_NONNULL_BEGIN

using namespace cv;
@class MMLData;

/**
 * MML 图片数据结构
 */
typedef struct {
    std::vector<cv::Mat> channels;//拆分后的色彩通道
    Mat alpha;//Alpha
} MMLImageData;

/**
 * MMLnormalization中，返回数据类型定义
*/
typedef NS_ENUM(NSUInteger, TMMLMatDataType) {
    TMMLMatDataType_32FC1,//返回mat为32f c1 格式
    TMMLMatDataType_8UC1,//返回mat为8u c1 格式
};

#pragma mark 填充Mat类型&struct
/**
 * MML margin数据结构，用于裁剪
*/
typedef struct {
    int top;
    int bottom;
    int left;
    int right;
} MMLMargin;

/**
 * MMLborder填充类型
*/
typedef NS_ENUM(int, TMMLBorderTypes) {
    TMMLBorderConstant    = BORDER_CONSTANT, //!< `iiiiii|abcdefgh|iiiiiii`  with some specified `i`
    TMMLBorderReplicate   = BORDER_REPLICATE, //!< `aaaaaa|abcdefgh|hhhhhhh`
    TMMLBorderReflect     = BORDER_REFLECT, //!< `fedcba|abcdefgh|hgfedcb`
    TMMLBorderWarp        = BORDER_WRAP, //!< `cdefgh|abcdefgh|abcdefg`
    TMMLBorderReflect_101 = BORDER_REFLECT_101, //!< `gfedcb|abcdefgh|gfedcba`
    TMMLBorderTransparent = BORDER_TRANSPARENT, //!< `uvwxyz|abcdefgh|ijklmno`
    TMMLBorderIsolated    = BORDER_ISOLATED, //!< do not look outside of ROI
    TMMLBorderDefault     = BORDER_DEFAULT, //!< same as BORDER_REFLECT_101
};

#pragma mark class define
/**
 * MML数据处理工具方法
*/
@interface MMLDataProcess : NSObject

/**
  上采样
 
  @param src 待采样mat
  @param samplingRate 采样比例
  @return sampling后的Mat数据，如果samplingRate是0，则为非法，会返回empty的mat
 */
+ (Mat)mml_subSamplingCVMat:(Mat)src rate:(double)samplingRate;
/**
 下采样

 @param src 待采样mat
 @param samplingRate 采样比例
 @return sampling后的Mat数据
*/
+ (Mat)mml_upSamplingCVMat:(Mat)src rate:(double)samplingRate;

/**
 UIimage -> Mat互转方法

 @param src 入参数图片数据
 @return 转换后的UIimage数据
*/
+ (UIImage *)mml_UIImageFromCVMat:(Mat)src;

/**
 Mat -> UIimage转换

 @param src 入参数图片数据
 @return 转换后的Mat数据
*/
+ (Mat)mml_CVMatFromUIImage:(UIImage *)src;


/**
 normalization方法

 @param src 入参数图片数据
 @param outputType 返回Mat数据类型
 @param alpha 转换系数，return = src * alpha + beta
 @param beta 转换偏移，return = src * alpha + beta
 @return Mat 返回转换后的数据。如果outputType是无法支持的类型，会返回empty的mat
*/
+ (Mat)mml_CVMatNormalization:(Mat)src
                   outputType:(TMMLMatDataType)outputType
                        alpha:(double)alpha
                         beta:(double)beta;

/**
 Mat Resize方法

 @param src 入参数图片数据
 @param size output的size
 @return Mat scale后的size
*/
+ (Mat)mml_scaleCVMat:(Mat)src toSize:(Size2i)size;

/**
 RGB的MML图片数据结构转Mat

 @param ycrcbData YCrCb数据
 @return Mat 转换后的Mat，如果ycrcbData通道数不是1、3、4则无法支持，会返回empty的mat
*/
+ (Mat)mml_imageRGBCVMatFromYCrCb:(MMLImageData)ycrcbData;

/**
 RGB的MML图片数据结构转Mat

 @param yuvData YUV数据
 @return Mat 转换后的Mat，如果ycrcbData通道数不是1、3、4则无法支持，会返回empty的mat
*/
+ (Mat)mml_imageRGBCVMatFromYUV:(MMLImageData)yuvData;

/**
 Mat转MML图片数据

 @param imageMat YCrCb数据
 @return MMLImageData RGB的MML图片数据结构，如果imageMat通道数不是1、3、4则无法支持，会返回empty的mat
*/
+ (MMLImageData)mml_YCrCbFromImageRGBCVMat:(Mat)imageMat;

/**
 Mat转MML图片数据

 @param imageMat YUV数据
 @return MMLImageData RGB的MML图片数据结构，如果imageMat通道数不是1、3、4则无法支持，会返回empty的mat
*/
+ (MMLImageData)mml_YUVFromImageRGBCVMat:(Mat)imageMat;

/**
 Mat -> float *转换

 @param inputMat Mat数据
 @param image_data float*数据,待填充
*/
+ (void)mml_convertCVMatData:(Mat)inputMat toFloatData:(float *)image_data;///Mat    >> float*

/**
 float * -> Mat转换

 @param image_data float*数据
 @param inputMat Mat数据，待填充
*/
+ (void)mml_convertFloatData:(float *)image_data toCVMatData:(Mat)inputMat;///float* >> Mat

/**
 Mat -> uint8_t *转换

 @param inputMat Mat数据
 @param image_data uint8_t*数据,待填充
*/
+ (void)mml_convertCVMatData:(Mat)inputMat toUint8Data:(uint8_t *)image_data;

/**
 uint8_t * -> Mat转换
 
 @param image_data uint8_t*数据
 @param inputMat Mat数据，待填充
*/
+ (void)mml_convertUint8Data:(uint8_t *)image_data toCVMatData:(Mat)inputMat;

/**
 剪裁Mat

 @param inputMat Mat数据
 @param rect 剪裁的区域
 @return Mat 剪裁后的Mat
*/
+ (Mat)mml_shearCVMat:(Mat)inputMat rect:(Rect2i)rect;

/**
 填充Mat

 @param inputMat Mat数据
 @param margin 填充的margin
 @param borderTypre 填充的方法
 @return Mat 剪裁后的Mat
*/
+ (Mat)mml_borderCVMat:(Mat)inputMat margin:(MMLMargin)margin type:(TMMLBorderTypes)borderTypre;


@end

NS_ASSUME_NONNULL_END
