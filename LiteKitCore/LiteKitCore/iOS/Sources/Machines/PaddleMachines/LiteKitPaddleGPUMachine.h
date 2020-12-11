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


/// Paddle GPU Machine
@interface LiteKitPaddleGPUMachine : LiteKitBaseMachine

/*
 * @brief Action = init paddle GPU Machine
 * @param - modelURL model file path
 * @param - paramPath params file path
 * @param - netType net type
 * @param - config origin ModelConfig
 * @param - error error message （ErrorDomain:LiteKitPaddleMachineInitErrorDomain ErrorCode:PMachineErrorCode）
 * @return instancetype
 */
- (instancetype __nullable)initWithModelPath:(NSString *)modelPath
                                   paramPath:(NSString *)paramPath
                                     netType:(NetType)netType
                                 modelConfig:(PaddleGPUConfig *)config
                                   withError:(NSError **)error;


/**
 init paddle GPU machine

 @param modelConfig model config
 @param netType net type
 @param error error message
 @return instancetype
 */
- (instancetype __nullable)initWithModelConfig:(PaddleGPUConfig *)modelConfig
                                       netType:(NetType)netType
                                     withError:(NSError**)error;

/**
 predict interface
 
 @param input input data
 @param callback output call back
 */
- (void)predictWithInputMatrix:(LiteKitInputMatrix *)input
               completionBlock:(LiteKitPaddleGPUPredicatOutputsCallback)callback;


/**
 clear machine memory
 */
- (void)clearMachine;

/**
 release machine memory,  release GPUMachine
 */
- (void)releasePaddleGPU;

/**
 reload machine
 */
- (void)reloadMachineWithError:(NSError **)error;

@end

NS_ASSUME_NONNULL_END
