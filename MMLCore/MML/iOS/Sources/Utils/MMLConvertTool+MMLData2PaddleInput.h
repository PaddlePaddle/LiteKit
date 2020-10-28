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
