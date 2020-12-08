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
#import "PaddleGPU.h"
#import "LiteKitInferenceEngineConfigProtocol.h"

NS_ASSUME_NONNULL_BEGIN

/// Paddle Machine的配置
@interface LiteKitPaddleConfig : NSObject <LiteKitInferenceEngineConfigProtocol>
/// machineType == LiteKitPaddleGPU时，可以配置GPU Inference Engine的配置，此时参数可选
@property (nonatomic, strong) PaddleGPUConfig *paddleGPUConfig;
/// machineType == LiteKitPaddleGPU时，必须配置GPU Inference Engine的net类型，此时参数必选
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
