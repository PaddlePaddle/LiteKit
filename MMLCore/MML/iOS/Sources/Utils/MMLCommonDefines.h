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
