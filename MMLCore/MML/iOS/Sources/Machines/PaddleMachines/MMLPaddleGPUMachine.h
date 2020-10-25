//
//  MMLPaddleGPUMachine.h
//  MML+Plugin
//
//  Created by Baidu. Co.,Ltd. on 2019/11/14.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "MMLBaseMachine.h"
#import "PaddleGPU.h"

@class MMLInputMatrix;

NS_ASSUME_NONNULL_BEGIN

typedef void (^MMLPaddleGPUPredicatOutputsCallback)(PaddleGPUResult *__nullable gpuPredicateOutputs, NSError *__nullable error);


/// Paddle GPU 的Machine
@interface MMLPaddleGPUMachine : MMLBaseMachine

/*
 * @brief Action = 初始化模型 模型操作工具Machine
 * @param - modelURL 模型文件URL（模型文件本身）
 * @param - paramPath params文件URL
 * @param - netType 网络类型
 * @param - config 原始ModelConfig
 * @param - error 错误容器 （ErrorDomain:MMLPaddleMachineInitErrorDomain ErrorCode:PMachineErrorCode）
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
- (void)predictWithInputMatrix:(MMLInputMatrix *)input
               completionBlock:(MMLPaddleGPUPredicatOutputsCallback)callback;


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
