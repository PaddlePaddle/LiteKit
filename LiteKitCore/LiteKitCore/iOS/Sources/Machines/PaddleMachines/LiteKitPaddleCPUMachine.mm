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


#import "LiteKitPaddleCPUMachine.h"
#import "LiteKitBaseMachine+Logger.h"
#import "LiteKitBaseMachine+Performancer.h"

#import "LiteKitData.h"
#import "PaddleCPU.h"

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

@interface LiteKitPaddleCPUMachine ()
@property (nonatomic, strong)PaddleCPU *paddleCPU;

@end

@implementation LiteKitPaddleCPUMachine

- (instancetype)initWithModelDir:(NSString *)modelDir
                       withError:(NSError**)error {
    if (self = [super init]) {
        if ([modelDir isKindOfClass:[NSString class]] &&
            ![modelDir isEqualToString:@""] ) {
            
            NSFileManager *fileManager = [NSFileManager defaultManager];
            
            if ( [fileManager fileExistsAtPath:modelDir] ) {
                
                PaddleCPUConfig *config = [[PaddleCPUConfig alloc]init];
                config.modelDir = modelDir;
                
                self.paddleCPU = [[PaddleCPU alloc] initWithConfig:config];
                
                NSError *loadError = nil;
                [self.paddleCPU loadWithError:&loadError];
                
                if (nil != loadError) {
                    // error
                    if (error != NULL) {
                        *error = [NSError errorWithDomain:LiteKitPaddleMachineInitErrorDomain code:(LiteKitMachineInitErrorCode)LiteKitMachineInitFailed userInfo:nil];
                        [self.logger errorLogMsg:[NSString stringWithFormat:@"error with detail msg, load error -- domain:%@, code:%ld, ext:%@", ( *error).domain.description, ( *error).code, ( *error).userInfo]];
                    }
                    self = nil;
                }
            } else {
                if (error != NULL) {
                    *error = [NSError errorWithDomain:LiteKitPaddleMachineInitErrorDomain code:(LiteKitMachineInitErrorCode)LiteKitModelFileNotFound userInfo:nil];
                }
                self = nil;
            }
        } else {
            //error
            if (error != NULL) {
                *error = [NSError errorWithDomain:LiteKitPaddleMachineInitErrorDomain code:(LiteKitMachineInitErrorCode)LiteKitIllegalModelFileURL userInfo:nil];
                [self.logger errorLogMsg:[NSString stringWithFormat:@"error with detail msg, file path error -- domain:%@, code:%ld, ext:%@", ( *error).domain.description, ( *error).code, ( *error).userInfo]];
            }
            self = nil;
        }
    }
    return self;
}


/*
 * @brief Action = 预测API
 * @param - input input容器
 * @param - error 错误容器 （ErrorDomain:LiteKitMachinPredicateErrorDomain ErrorCode:PPredicateErrorCode）
 * @return LiteKitPredicateOutputs* 预测结果容器
 */
- (void)predictWithInputData:(LiteKitData * __nonnull)aInputData completionBlock:(LiteKitMachinePredictCompletionBlock)aBlock {
    if (aInputData.type != TLiteKitDataTypeLiteKitShapedData) {
        NSError *aError = [NSError errorWithDomain:LiteKitPaddleMachinePredicateErrorDomain code:0 userInfo:nil];
        aBlock(nil, aError);
        return;
    }
    
    NSTimeInterval start = [[NSDate date] timeIntervalSince1970];
    
    LiteKitShapedData *input = aInputData.litekitShapedData;
    PaddleCPUInput *predictInput = [[PaddleCPUInput alloc] initWithData:input.data
                                                                 dataSize:input.dataSize
                                                                     dims:input.dim];
    
    NSError *predictError = nil;
    PaddleCPUResult *result = [self.paddleCPU predictWithInput:predictInput error:&predictError];
    
    LiteKitShapedData *outputShapedData = [[LiteKitShapedData alloc] initWithData:result.data
                                                                 dataSize:result.dataSize
                                                                     dims:result.dim];
    LiteKitData *predictOutput = [[LiteKitData alloc] initWithData:outputShapedData type:TLiteKitDataTypeLiteKitShapedData];
    
    NSTimeInterval end = [[NSDate date] timeIntervalSince1970];
    [self.logger performanceInfoLogMsg:[NSString stringWithFormat:@"paddle gpu predict time: %f ms", (end - start) * 1000]];
    
    
    aBlock(predictOutput, nil);
}


@end
