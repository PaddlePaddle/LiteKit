//
//  ViewController+Load.h
//  MMLDemo
//
//  Created by Baidu Co.Ltd. on 2020/3/10.
//  Copyright © 2020 Baidu. All rights reserved.
//

#import "ViewController.h"

///mml  header
#import "mml_inference_api.h"

NS_ASSUME_NONNULL_BEGIN

@interface ViewController(MMLCore_CPP)

/// CPU
- (mml_framework::MMLMachineService *)loadMMLWithModelDir_LITE_MODEL_FROM_BUFFER:(NSString *)modelDir;
- (mml_framework::MMLMachineService *)loadMMLWithModelDir_LITE_MODEL_FROM_FILE:(NSString *)modelDir;

- (std::shared_ptr<mml_framework::MMLMachineService>)loadMMLWithModelDir_Shared:(NSString *)modelDir;

/// GPU
- (std::shared_ptr<mml_framework::MMLMachineService>)loadMMLWithModelDir_GPU_CPP:(NSString *)modelDir;
@end

NS_ASSUME_NONNULL_END
