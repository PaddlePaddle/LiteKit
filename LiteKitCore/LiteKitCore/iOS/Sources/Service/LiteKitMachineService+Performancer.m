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


#import "LiteKitMachineService+Performancer.h"
#import "LiteKitBaseMachine+Performancer.h"


@implementation LiteKitMachineService (Performancer)

- (void)loadMachineWithConfig:(LiteKitMachineConfig * __nonnull)aConfig
             performanceBlock:(LiteKitMachineLoadPerformanceBlock)aBlock {
    NSTimeInterval loadStart = [[NSDate date] timeIntervalSince1970];
    [self loadMachineWithConfig:aConfig completionBlock:^(LiteKitMachine * _Nullable machine, NSError * _Nullable error) {
        NSTimeInterval loadEnd = [[NSDate date] timeIntervalSince1970];
        LiteKitPerformanceProfiler *profiler = [[LiteKitPerformanceProfiler alloc] init];
        [profiler appendPerformanceData:[NSString stringWithFormat:@"%.3f", (loadEnd - loadStart) * 1000]
                                    key:LiteKitPerformanceLoadTimeForInterface];
        if (!error) {
            [profiler appendPerformanceData:self.litekitMachine.loadTime key:LiteKitPerformanceLoadTimeForInferenceEngine];
            [profiler appendPerformanceData:self.litekitMachine.readMetalResourceTime key:LiteKitPerformancePredictTimeForInferenceEngine];
        }
        aBlock(machine, profiler, error);
    }];
}


@end
