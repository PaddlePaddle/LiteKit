/*
 Copyright © 2020 Baidu, Inc. All Rights Reserved.

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


/// Machine public header
#ifndef LiteKitMachineCommonHeader_h
#define LiteKitMachineCommonHeader_h

#import "LiteKitCommonDefines.h"

/// error domain
#define LiteKitPaddleMachineInitErrorDomain       @"PaddleMachineInitError"
#define LiteKitPaddleMachinePredicateErrorDomain  @"PaddleMachinePredicateError"
#define LiteKitBMLMachinePredicateErrorDomain     @"BMLMachinePredicateError"

/// error key
/// predict period error userinfo key
#define LiteKitMachinePredictErrorExtKey          @" litekit_error_ext_key"

#define SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)

/// error code
typedef NS_ENUM(NSInteger, LiteKitMachineInitErrorCode) {
    LiteKitMachineInitFailed = 0,       // machine init fail
    LiteKitMachineLoadFailed = 1,       // machine load fail
    LiteKitModelFileNotFound = 2,       // model file not found
    LiteKitIllegalModelFileURL = 3,     // model file path not valid
    LiteKitIllegalOSVersion = 4,        // error OS version
    LiteKitIllegalModelConfig = 5       // error config
};

/// error code
typedef NS_ENUM(NSInteger, LiteKitMachinePredicateErrorCode) {
    LiteKitMachinePredicateDestroyed = 0,                   // machine already destroyed
    LiteKitMachinePredicateInputPixelTooLarge = 1,          // input size too large
    LiteKitMachinePredicatePredictError = 2,                // predict error
    LiteKitMachinePredicateLoadError = 3,                   // load error
    LiteKitMachinePredicateUpdateDimError = 4,              // update dim fail
    LiteKitMachinePredicateConvertTextureError = 5,         // convert texture fail
    LiteKitMachinePredicateInputDataError = 6,              // input data not valid
    LiteKitMachinePredicateNotSupportSimulator = 7,         // simulator not support
    LiteKitMachinePredicateNotSupportArchitecture = 8,      // architecture not support
    LiteKitMachinePredicateMachineNotReady = 9,             // Machine not ready
    LiteKitMachinePredicateMachineReloadError = 10          // model reload fail
};

#endif /* LiteKitMachineCommonHeader_h */
