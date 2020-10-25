//
//  MMLMachineService+Performancer.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/12/9.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import <MML/MML.h>
#import "MMLPerformanceProfiler.h"
#import "MMLMachineService.h"

NS_ASSUME_NONNULL_BEGIN
/// @param machine 加载成功的machine
/// @param performanceProfiler performance信息
/// @param error error信息
typedef void (^MMLMachineLoadPerformanceBlock)(MMLMachine * __nullable machine,
                                               MMLPerformanceProfiler * __nullable performanceProfiler,
                                               NSError * __nullable error);

/// 带performance信息的Service分类
@interface MMLMachineService (Performancer)

/// 加载带performance信息回调的machine
/// @param aConfig 配置
/// @param aBlock 带performance信息的回调
- (void)loadMachineWithConfig:(MMLMachineConfig * __nonnull)aConfig performanceBlock:(MMLMachineLoadPerformanceBlock)aBlock;
@end

NS_ASSUME_NONNULL_END
