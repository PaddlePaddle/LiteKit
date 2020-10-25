//
//  MMLBaseMachine+Performancer.m
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/12/9.
//  Copyright © 2019 Baidu. All rights reserved.
//

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
