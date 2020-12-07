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


#import "MMLBaseMachine+Performancer.h"
#import <objc/runtime.h>


@implementation MMLBaseMachine (Performancer)

- (void)setLoadTime:(NSString *)loadTime {
    objc_setAssociatedObject(self, @selector(loadTime), loadTime, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (NSString *)loadTime {
    return objc_getAssociatedObject(self, _cmd);
}

- (void)setPredictTime:(NSString *)predictTime {
    objc_setAssociatedObject(self, @selector(predictTime), predictTime, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (NSString *)predictTime {
    return objc_getAssociatedObject(self, _cmd);
}

- (void)setReadMetalResourceTime:(NSString *)readMetalResourceTime {
    objc_setAssociatedObject(self, @selector(readMetalResourceTime), readMetalResourceTime, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (NSString *)readMetalResourceTime {
    return objc_getAssociatedObject(self, _cmd);
}


@end
