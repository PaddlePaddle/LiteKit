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
