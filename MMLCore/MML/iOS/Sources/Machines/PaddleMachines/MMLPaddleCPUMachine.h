//
//  MMLPaddleCPUMachine.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2020/2/11.
//  Copyright © 2020 Baidu. All rights reserved.
//

#import <MML/MML.h>
#import "MMLBaseMachine.h"

NS_ASSUME_NONNULL_BEGIN
///machine
@class PaddleCPU;
@class PaddleCPUConfig;
///input
@class MMLShapedData;
///output
@class MMLData;

@interface MMLPaddleCPUMachine : MMLBaseMachine

/*
 * @brief Action = 初始化模型 模型操作工具Machine
 * @param - modelDir 模型文件路径
 * @param - error 错误容器 （ErrorDomain:MMLPaddleMachineInitErrorDomain ErrorCode:PMachineErrorCode）
 * @return instancetype
 */
- (instancetype)initWithModelDir:(NSString *)modelDir
                       withError:(NSError**)error;

@end

NS_ASSUME_NONNULL_END
