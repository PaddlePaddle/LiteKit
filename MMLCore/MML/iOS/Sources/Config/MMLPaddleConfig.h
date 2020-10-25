//
//  MMLPaddleConfig.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/19.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "PaddleGPU.h"
#import "MMLInferenceEngineConfigProtocol.h"

NS_ASSUME_NONNULL_BEGIN

/// Paddle Machine的配置
@interface MMLPaddleConfig : NSObject <MMLInferenceEngineConfigProtocol>
/// machineType == MMLPaddleGPU时，可以配置GPU Inference Engine的配置，此时参数可选
@property (nonatomic, strong) PaddleGPUConfig *paddleGPUConfig;
/// machineType == MMLPaddleGPU时，必须配置GPU Inference Engine的net类型，此时参数必选
@property (nonatomic, assign) NetType netType;

/// fluid模型内存指针
@property (nonatomic, assign) unsigned char * modelPointer;
/// fluid参数内存指针
@property (nonatomic, assign) unsigned char * paramPointer;
/// 模型内存大小
@property (nonatomic, assign) size_t modelSize;
/// 参数内存大小
@property (nonatomic, assign) size_t paramSize;
@end

NS_ASSUME_NONNULL_END
