//
//  MMLCommonDefines.h
//  MML
//
//  Created by Baidu. Co.,Ltd. on 2019/11/15.
//  Copyright © 2019 Baidu. All rights reserved.
//

/// MML 的通用头文件
#ifndef MMLCommonDefines_h
#define MMLCommonDefines_h
#import <Foundation/Foundation.h>
#import "MMLLoggerProtocol.h"

#if __has_include("MMLLogger.h")
#import "MMLLogger.h"
#endif

#ifdef DEBUG
#if __has_include("MMLLogger.h")
#   define NSLog(frmt, ...) LogVerbose((@"[Method %s] [Line %d] " frmt), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#   define NSLog(fmt, ...) NSLog((@"[File %s] [Method %s] [Line %d] " fmt), __FILE__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif
#else
#   define NSLog(...)
#endif

/// logger Tag name
#define MMLMachineServiceLoggerTag  @"MachineService"
#define MMLMachineLoggerTag @"Machine"
#define MMLTaskQueueLoggerTag @"TaskQueue"

#endif /* MMLCommonDefines_h */
