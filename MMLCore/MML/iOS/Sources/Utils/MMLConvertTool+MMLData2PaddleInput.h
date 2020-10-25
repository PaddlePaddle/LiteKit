//
//  MMLConvertTool+MMLData2PaddleInput.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/21.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MMLConvertTool.h"
#import "MMLData.h"

NS_ASSUME_NONNULL_BEGIN
/**
 * MMLConvertTool MMLData >> paddle input
*/
@interface MMLConvertTool(MMLData2PaddleInput)

/**
 UIImage -> Paddle Input

 @param image 输入的image
 @return 转换后的MMLInputMatrix
*/
+ (MMLInputMatrix *)inputMatrixConvertFromImage:(UIImage *)image;

/**
 UIImageURL -> Paddle Input

 @param imageURL 图片地址
 @return 转换后的MMLInputMatrix
*/
+ (MMLInputMatrix *)inputMatrixConvertFromImageURL:(NSString *)imageURL;

/**
 pixelBuffer -> Paddle Input

 @param pixelBuffer 图片的CVPixelBufferRef格式
 @return 转换后的MMLInputMatrix
*/
+ (MMLInputMatrix *)inputMatrixConvertFromCVPixelBuffer:(CVPixelBufferRef)pixelBuffer;

/**
 multiArray -> Paddle Input

 @param multiArray 图片的MLMultiArray格式
 @return 转换后的MMLInputMatrix
*/
+ (MMLInputMatrix *)inputMatrixConvertFromMultiArray:(MLMultiArray *)multiArray;

/**
 MMLShapedData -> Paddle Input

 @param shapedData 图片的MMLShapedData格式
 @return 转换后的MMLInputMatrix
*/
+ (MMLInputMatrix *)inputMatrixConvertFromMMLShapedData:(MMLShapedData *)shapedData;

@end

NS_ASSUME_NONNULL_END
