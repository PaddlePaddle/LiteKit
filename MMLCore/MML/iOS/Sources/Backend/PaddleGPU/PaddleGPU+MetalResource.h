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
#import "PaddleGPU.h"

NS_ASSUME_NONNULL_BEGIN
// 解析metal Error Domain
FOUNDATION_EXPORT NSString * _Nonnull const PaddleGPUParseMetalErrorDomain;

// PaddleGPU的MetalLib路径解析相关状态码
typedef NS_ENUM(NSInteger, PaddleGPUMetalLibCode) {
    PaddleGPUMetalLibCodeNotExistZipPlist   = 0,// 无外层zip plist
    PaddleGPUMetalLibCodeZipPlistError      = 1,// 外层zip plist 错误
    PaddleGPUMetalLibCodeNotExistZip        = 2,// 外层zip 不存在
    PaddleGPUMetalLibCodeBundlePlistError   = 3,// 内层bundle plist 错误
    PaddleGPUMetalLibCodeRemoveBundleError  = 4,// 删除内层bundle 失败
    PaddleGPUMetalLibCodeUnzipError         = 5,// 解压zip 失败
    PaddleGPUMetalLibCodeNotExistBundle     = 6 // 不存在内层bundle
};

/// 获取PaddleGPU 自定义的MetalLib
@interface PaddleGPU (MetalResource)

/**
 获取Paddle自定义的MetalLib路径
 @param error 错误信息，具体的错误code是TPaddleGPUMetalLibCode中的枚举值
 @return paddle自定义的MetalLib路径
*/
+ (NSString *)pm_customMetalLibResourceWithError:(NSError **)error;

@end

NS_ASSUME_NONNULL_END
