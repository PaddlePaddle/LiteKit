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
#import "LiteKitConvertTool.h"
#import "LiteKitData.h"

NS_ASSUME_NONNULL_BEGIN
/**
 * LiteKitConvertTool LiteKitData >> paddle input
*/
@interface LiteKitConvertTool(LiteKitData2PaddleInput)

/**
 UIImage -> Paddle Input

 @param image 输入的image
 @return 转换后的LiteKitInputMatrix
*/
+ (LiteKitInputMatrix *)inputMatrixConvertFromImage:(UIImage *)image;

/**
 UIImageURL -> Paddle Input

 @param imageURL 图片地址
 @return 转换后的LiteKitInputMatrix
*/
+ (LiteKitInputMatrix *)inputMatrixConvertFromImageURL:(NSString *)imageURL;

/**
 pixelBuffer -> Paddle Input

 @param pixelBuffer 图片的CVPixelBufferRef格式
 @return 转换后的LiteKitInputMatrix
*/
+ (LiteKitInputMatrix *)inputMatrixConvertFromCVPixelBuffer:(CVPixelBufferRef)pixelBuffer;

/**
 multiArray -> Paddle Input

 @param multiArray 图片的MLMultiArray格式
 @return 转换后的LiteKitInputMatrix
*/
+ (LiteKitInputMatrix *)inputMatrixConvertFromMultiArray:(MLMultiArray *)multiArray;

/**
 LiteKitShapedData -> Paddle Input

 @param shapedData 图片的LiteKitShapedData格式
 @return 转换后的LiteKitInputMatrix
*/
+ (LiteKitInputMatrix *)inputMatrixConvertFromLiteKitShapedData:(LiteKitShapedData *)shapedData;

@end

NS_ASSUME_NONNULL_END
