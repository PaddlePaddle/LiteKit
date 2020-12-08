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

NS_ASSUME_NONNULL_BEGIN

@class PaddleCPUConfig;
@class PaddleCPUResult;
@class PaddleCPUInput;
@interface PaddleCPU : NSObject


/**
 @b 创建对象

 @param config 配置
 @return PaddleCPU对象
 */
- (instancetype)initWithConfig:(PaddleCPUConfig *)config;

/**
 @b 加载machine
 
 @param error 错误信息
 */
- (void)loadWithError:(NSError *__autoreleasing  _Nullable * _Nullable)error;


/**
 @b 进行预测, 预处理 means 值为 0, scale 值为 1

 @param input 输入图像
 @param error 返回错误
 @return 预测结果
 */
- (PaddleCPUResult *)predictWithInput:(PaddleCPUInput *)input
                                 error:(NSError **) error;


@end


#pragma mark - @Class PaddleCPUConfig
@interface  PaddleCPUConfig: NSObject
@property (nonatomic, strong) NSString *modelDir;

@end



#pragma mark - @Class PaddleCPUShapedData
@interface  PaddleCPUShapedData: NSObject
///数据float*
@property (nonatomic, readonly) float *data;
///数据size
@property (nonatomic, readonly) NSInteger dataSize;
///数据dims
@property (nonatomic, readonly) NSArray <NSNumber *> *dim;

/**
 * 初始化方法
 
 @param data 初始化数据
 @param dataSize 数据长度
 @param dim 数据维度
 @return 初始化的数据
 */
- (instancetype)initWithData:(float *)data
                    dataSize:(NSInteger)dataSize
                        dims:(NSArray <NSNumber *> *)dim;
@end

#pragma mark - @Class PaddleCPUResult
@interface  PaddleCPUResult: PaddleCPUShapedData

@end

#pragma mark - @Class PaddleCPUInput
@interface  PaddleCPUInput: PaddleCPUShapedData

@end

NS_ASSUME_NONNULL_END
