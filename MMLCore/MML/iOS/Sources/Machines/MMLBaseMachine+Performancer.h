//
//  MMLBaseMachine+Performancer.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/12/9.
//  Copyright © 2019 Baidu. All rights reserved.
//


#import "MMLBaseMachine.h"

NS_ASSUME_NONNULL_BEGIN

/// 性能数据
@interface MMLBaseMachine (Performancer)

/// 纯inferenceEngine的加载时间
@property (nonatomic, copy) NSString *loadTime;
/// 纯inferenceEngine的预测时间
@property (nonatomic, copy) NSString *predictTime;
/// 读取paddle的metal资源时间
@property (nonatomic, copy) NSString *readMetalResourceTime;

@end

NS_ASSUME_NONNULL_END
