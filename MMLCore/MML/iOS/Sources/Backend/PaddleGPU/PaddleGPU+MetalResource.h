//
//  PaddleGPU+MetalResource.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/6.
//  Copyright © 2019 Baidu. All rights reserved.
//

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
