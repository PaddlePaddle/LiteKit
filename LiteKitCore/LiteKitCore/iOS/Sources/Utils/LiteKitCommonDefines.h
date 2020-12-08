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


/// LiteKit 的通用头文件
#ifndef LiteKitCommonDefines_h
#define LiteKitCommonDefines_h
#import <Foundation/Foundation.h>
#import "LiteKitLoggerProtocol.h"

#if __has_include("LiteKitLogger.h")
#import "LiteKitLogger.h"
#endif

#ifdef DEBUG
#if __has_include("LiteKitLogger.h")
#   define NSLog(frmt, ...) LogVerbose((@"[Method %s] [Line %d] " frmt), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#   define NSLog(fmt, ...) NSLog((@"[File %s] [Method %s] [Line %d] " fmt), __FILE__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif
#else
#   define NSLog(...)
#endif

/// logger Tag name
#define LiteKitMachineServiceLoggerTag  @"MachineService"
#define LiteKitMachineLoggerTag @"Machine"
#define LiteKitTaskQueueLoggerTag @"TaskQueue"

#endif /* LiteKitCommonDefines_h */
