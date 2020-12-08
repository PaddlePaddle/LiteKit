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



#import "PaddleCPU.h"

static NSString *TLiteKit_PaddleCPU_LoadError = @"TLiteKit_PaddleCPU_LoadError";
#if __has_include("PaddleLite/paddle_api.h")
#include "PaddleLite/paddle_api.h"


@interface PaddleCPUResult ()
///数据float*
@property (nonatomic, assign) float *data;
///数据size
@property (nonatomic, assign) NSInteger dataSize;
///数据dims
@property (nonatomic, strong) NSArray <NSNumber *> *dim;
@end

@interface PaddleCPU ()
@property (nonatomic, strong) PaddleCPUConfig *config;
@property (nonatomic) std::shared_ptr<paddle::lite_api::PaddlePredictor> predictor;
@end

@implementation PaddleCPU


- (instancetype)initWithConfig:(PaddleCPUConfig *)config {
    self = [super init];
    if (self) {
        self.config = config;
    }
    return self;
}

- (void)loadWithError:(NSError *__autoreleasing  _Nullable * _Nullable)error {
    std::string model_dir = std::string([self.config.modelDir UTF8String]);
    
    // 1. Set MobileConfig
    paddle::lite_api::MobileConfig config;
    config.set_model_from_file(model_dir);

    // 2. Create PaddlePredictor by MobileConfig
    std::shared_ptr<paddle::lite_api::PaddlePredictor> predictor =
    paddle::lite_api::CreatePaddlePredictor<paddle::lite_api::MobileConfig>(config);
    
    if ( NULL != predictor ) {
        self.predictor = predictor;
    } else {
        if ( NULL != error) {
            *error = [NSError errorWithDomain:TLiteKit_PaddleCPU_LoadError code:0 userInfo:nil];
        }
    }
}

- (PaddleCPUResult *)predictWithInput:(PaddleCPUInput *)input
                                 error:(NSError **) error {
    // 3. Prepare input data
    paddle::lite_api::shape_t shape;
    for (int i = 0; i < [input.dim count]; i++) {
        shape.push_back([input.dim[i] integerValue]);
    }
    
    std::unique_ptr<paddle::lite_api::Tensor> input_tensor(self.predictor->GetInput(0));
    input_tensor->Resize(shape);
    auto *data = input_tensor->mutable_data<float>();
    memcpy(data, input.data, input.dataSize*sizeof(float));
    
    // 4. Run predictor
    self.predictor->Run();

    // 5. Get output
    std::unique_ptr<const paddle::lite_api::Tensor> output_tensor(self.predictor->GetOutput(0));
    PaddleCPUResult *result = [[PaddleCPUResult alloc] init];
    //datasize
    result.dataSize = litekit_ShapeProduction(output_tensor->shape());
    ///data
    result.data = (float *)malloc(sizeof(float)*result.dataSize);
    memcpy(result.data, output_tensor->data<float>(), result.dataSize*sizeof(float));
    ///dim
    NSMutableArray<NSNumber *> *dim = [NSMutableArray arrayWithCapacity:output_tensor->shape().size()];
    for ( int i = 0; i < output_tensor->shape().size(); i++) {
        [dim addObject:@(output_tensor->shape().at(i))];
    }
    result.dim = [dim copy];
    
    return result;
}

NSInteger litekit_ShapeProduction(const paddle::lite_api::shape_t& shape) {
  NSInteger res = 1;
  for (auto i : shape) res *= i;
  return res;
}

@end
#else // else __has_include(<PaddleLite/paddle_api.h>)
@implementation PaddleCPU

- (instancetype)initWithConfig:(PaddleCPUConfig *)config {
    return nil;
}

- (void)loadWithError:(NSError *__autoreleasing  _Nullable * _Nullable)error {
    if ( NULL != error) {
        *error = [NSError errorWithDomain:TLiteKit_PaddleCPU_LoadError code:0 userInfo:nil];
    }
}

- (PaddleCPUResult *)predictWithInput:(PaddleCPUInput *)input
                                 error:(NSError **) error {
    return nil;
}

@end
#endif // end __has_include(<PaddleLite/paddle_api.h>)




#pragma mark - @Class PaddleCPUConfig
@implementation PaddleCPUConfig

@end


#pragma mark - @Class PaddleCPUShapedData
@implementation  PaddleCPUShapedData
- (instancetype)initWithData:(float *)data
                    dataSize:(NSInteger)dataSize
                        dims:(NSArray <NSNumber *> *)dim {
    self = [super init];
    if (self) {
        _data = (float *)malloc(dataSize * sizeof(float));
        memcpy(_data, data, dataSize * sizeof(float));
        _dataSize = dataSize;
        _dim = [dim copy];
    }
    return self;
}

- (void)dealloc {
    if (_data != NULL) {
        free(_data);
    }
}
@end

#pragma mark - @Class PaddleCPUResult
@implementation  PaddleCPUResult
@synthesize data;
@synthesize dataSize;
@synthesize dim;
@end

#pragma mark - @Class PaddleCPUInput
@implementation  PaddleCPUInput

@end
