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
#import "PaddleGPU.h"

NS_ASSUME_NONNULL_BEGIN
// resolving metal Error Domain
FOUNDATION_EXPORT NSString * _Nonnull const PaddleGPUParseMetalErrorDomain;

// PaddleGPU MetalLib path state code
typedef NS_ENUM(NSInteger, PaddleGPUMetalLibCode) {
    PaddleGPUMetalLibCodeNotExistZipPlist   = 0,// no outer zip plist
    PaddleGPUMetalLibCodeZipPlistError      = 1,// outer zip plist error
    PaddleGPUMetalLibCodeNotExistZip        = 2,// outer zip not exist
    PaddleGPUMetalLibCodeBundlePlistError   = 3,// inner bundle plist error
    PaddleGPUMetalLibCodeRemoveBundleError  = 4,// delete inner bundle fail
    PaddleGPUMetalLibCodeUnzipError         = 5,// unzip fail
    PaddleGPUMetalLibCodeNotExistBundle     = 6 // inner bundle not exist
};

/// get MetalLib in PaddleGPU
@interface PaddleGPU (MetalResource)

/**
 get custom MetalLib path from Paddle
 @param error message，error code is from enum TPaddleGPUMetalLibCode
 @return custom MetalLib Path
*/
+ (NSString *)pm_customMetalLibResourceWithError:(NSError **)error;

@end

NS_ASSUME_NONNULL_END
