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

NS_ASSUME_NONNULL_BEGIN

@class PaddleCPUConfig;
@class PaddleCPUResult;
@class PaddleCPUInput;
@interface PaddleCPU : NSObject


/**
 @b create instance

 @param config config
 @return PaddleCPU Instance
 */
- (instancetype)initWithConfig:(PaddleCPUConfig *)config;

/**
 @b load machine
 
 @param error error message
 */
- (void)loadWithError:(NSError *__autoreleasing  _Nullable * _Nullable)error;


/**
 @b predict, preprocess means = 0, scale = 1

 @param input input image
 @param error return error
 @return predict result
 */
- (PaddleCPUResult *)predictWithInput:(PaddleCPUInput *)input
                                 error:(NSError **) error;


@end


#pragma mark - @Class PaddleCPUConfig
@interface  PaddleCPUConfig: NSObject
@property (nonatomic, strong) NSString *modelDir;

@end



#pragma mark - @Class PaddleCPUShapedData
@interface  PaddleCPUShapedData: NSObject
///data float*
@property (nonatomic, readonly) float *data;
///data size
@property (nonatomic, readonly) NSInteger dataSize;
///data dims
@property (nonatomic, readonly) NSArray <NSNumber *> *dim;

/**
 * Init
 
 @param data init data
 @param dataSize data length
 @param dim data dims
 @return initeds data
 */
- (instancetype)initWithData:(float *)data
                    dataSize:(NSInteger)dataSize
                        dims:(NSArray <NSNumber *> *)dim;
@end

#pragma mark - @Class PaddleCPUResult
@interface  PaddleCPUResult: PaddleCPUShapedData

@end

#pragma mark - @Class PaddleCPUInput
@interface  PaddleCPUInput: PaddleCPUShapedData

@end

NS_ASSUME_NONNULL_END
