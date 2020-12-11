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


#import <Foundation/Foundation.h>
#import "LiteKitBaseMachine.h"
#import "LiteKitMachineConfig.h"
#import "LiteKitData.h"
#import "LiteKitPerformanceProfiler.h"

NS_ASSUME_NONNULL_BEGIN
/// load period domain
FOUNDATION_EXPORT NSString * const LiteKitMachineServiceLoadErrorDomain;
/// error message key
FOUNDATION_EXPORT NSString * const LiteKitMachineServiceErrorExtKey;

/// service load period error enum
typedef NS_ENUM(NSInteger,LiteKitMachineServiceLoadErrorCode) {
    LiteKitMachineServiceLoadMachineTypeError = 0,      // error machine type
    LiteKitMachineServiceLoadNotSupportSimulator,       // Simulator not support
    LiteKitMachineServiceLoadNotSupportArchitecture,    // Architecture not support
    LiteKitMachineServiceLoadWrongConfig,               // error config
    LiteKitMachineServiceLoadNoModelFile,               // model file not found
    LiteKitMachineServiceLoadNoModelPointer,            // model pointer is null
};


/// @param machine succeed loaded machine
/// @param error error message
typedef void (^LiteKitMachineLoadCompletionBlock)(LiteKitMachine * __nullable machine, NSError * __nullable error);


/// Machine Service class，charge of Machine load、run and release
@interface LiteKitMachineService : NSObject

/// current machine
@property (nonatomic, strong, readonly) LiteKitBaseMachine * __nullable litekitMachine;
/// Service identifier
@property (nonatomic, copy, readonly) NSString *machineServiceId;
/// performance data
@property (nonatomic, strong, readonly) LiteKitPerformanceProfiler *performanceDataMap;

#pragma mark - Load

/// sync load Machine with conofig
/// @param aConfig LiteKit config
/// @param aError error message
- (LiteKitBaseMachine * __nullable)loadMachineWithConfig:(LiteKitMachineConfig * __nonnull)aConfig error:(NSError **)aError;

/// async
/// @param aConfig LiteKit config
/// @param aBlock complete callback
- (void)loadMachineWithConfig:(LiteKitMachineConfig * __nonnull)aConfig completionBlock:(LiteKitMachineLoadCompletionBlock)aBlock;


@end

NS_ASSUME_NONNULL_END
