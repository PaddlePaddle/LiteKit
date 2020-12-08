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
#import "LiteKitBaseMachine.h"
#import "PaddleGPU.h"

@class LiteKitInputMatrix;

NS_ASSUME_NONNULL_BEGIN

typedef void (^LiteKitPaddleGPUPredicatOutputsCallback)(PaddleGPUResult *__nullable gpuPredicateOutputs, NSError *__nullable error);


/// Paddle GPU 的Machine
@interface LiteKitPaddleGPUMachine : LiteKitBaseMachine

/*
 * @brief Action = 初始化模型 模型操作工具Machine
 * @param - modelURL 模型文件URL（模型文件本身）
 * @param - paramPath params文件URL
 * @param - netType 网络类型
 * @param - config 原始ModelConfig
 * @param - error 错误容器 （ErrorDomain:LiteKitPaddleMachineInitErrorDomain ErrorCode:PMachineErrorCode）
 * @return instancetype
 */
- (instancetype __nullable)initWithModelPath:(NSString *)modelPath
                                   paramPath:(NSString *)paramPath
                                     netType:(NetType)netType
                                 modelConfig:(PaddleGPUConfig *)config
                                   withError:(NSError **)error;


/**
 初始化模型

 @param modelConfig modelconfig配置
 @param netType 网络类型
 @param error 错误容器
 @return instancetype
 */
- (instancetype __nullable)initWithModelConfig:(PaddleGPUConfig *)modelConfig
                                       netType:(NetType)netType
                                     withError:(NSError**)error;

/**
 预测接口
 
 @param input 输入容器
 @param callback 结果容器
 */
- (void)predictWithInputMatrix:(LiteKitInputMatrix *)input
               completionBlock:(LiteKitPaddleGPUPredicatOutputsCallback)callback;


/**
 清除machine内存
 */
- (void)clearMachine;

/**
 清除machine内存,且释放GPUMachine
 */
- (void)releasePaddleGPU;

/**
 重新load machine
 */
- (void)reloadMachineWithError:(NSError **)error;

@end

NS_ASSUME_NONNULL_END
