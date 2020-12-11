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
#import "LiteKitData.h"
#import "LiteKitMachine.h"
#import "LiteKitMachineCommonHeader.h"
#import "LiteKitPerformanceProfiler.h"

// error domain
FOUNDATION_EXPORT NSString * _Nonnull const LiteKitMachinInitErrorDomain;//machine init error Domain
FOUNDATION_EXPORT NSString * _Nonnull const LiteKitMachinPredicateErrorDomain;//predict Error Domain

NS_ASSUME_NONNULL_BEGIN

/// @param outputData output data
/// @param error error info
typedef void (^LiteKitMachinePredictCompletionBlock)(LiteKitData * __nullable outputData, NSError * __nullable error);


/// @param outputData output data
/// @param performanceProfiler performance info
/// @param error error info
typedef void (^LiteKitMachinePredictPerformanceBlock)(LiteKitData * __nullable outputData,
                                                  LiteKitPerformanceProfiler * __nullable performanceProfiler,
                                                  NSError * __nullable error);

/// Machine base class
@interface LiteKitBaseMachine : LiteKitMachine

/// machine unique ID
@property (nonatomic, copy, readonly) NSString * machineId;

#pragma mrak - Predict
/// sync
/// @param aInputData LiteKitData input data
/// @param aError error message
- (LiteKitData * __nullable)predictWithInputData:(LiteKitData * __nonnull)aInputData error:(NSError **)aError;

/// async
/// @param aInputData LiteKitData input data
/// @param aBlock complete block
- (void)predictWithInputData:(LiteKitData * __nonnull)aInputData completionBlock:(LiteKitMachinePredictCompletionBlock)aBlock;

/// predict with performance message
/// @param aInputData input data
/// @param aBlock callback with performance
- (void)predictWithInputData:(LiteKitData * __nonnull)aInputData performanceBlock:(LiteKitMachinePredictPerformanceBlock)aBlock;

#pragma mark - Clear

/// clear machine memory，not release Machine
- (void)clearMachine;

#pragma mark - Release

/// release Machine,set Machine null
- (void)releaseMachine;

@end

NS_ASSUME_NONNULL_END
