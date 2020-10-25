//
//  MMLMachineService+Performancer.m
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/12/9.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import "MMLMachineService+Performancer.h"
#import "MMLBaseMachine+Performancer.h"


@implementation MMLMachineService (Performancer)

- (void)loadMachineWithConfig:(MMLMachineConfig * __nonnull)aConfig
             performanceBlock:(MMLMachineLoadPerformanceBlock)aBlock {
    NSTimeInterval loadStart = [[NSDate date] timeIntervalSince1970];
    [self loadMachineWithConfig:aConfig completionBlock:^(MMLMachine * _Nullable machine, NSError * _Nullable error) {
        NSTimeInterval loadEnd = [[NSDate date] timeIntervalSince1970];
        MMLPerformanceProfiler *profiler = [[MMLPerformanceProfiler alloc] init];
        [profiler appendPerformanceData:[NSString stringWithFormat:@"%.3f", (loadEnd - loadStart) * 1000]
                                    key:MMLPerformanceLoadTimeForInterface];
        if (!error) {
            [profiler appendPerformanceData:self.mmlMachine.loadTime key:MMLPerformanceLoadTimeForInferenceEngine];
            [profiler appendPerformanceData:self.mmlMachine.readMetalResourceTime key:MMLPerformancePredictTimeForInferenceEngine];
        }
        aBlock(machine, profiler, error);
    }];
}


@end
