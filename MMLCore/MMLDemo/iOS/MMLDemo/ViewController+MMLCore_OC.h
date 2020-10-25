//
//  ViewController+Load_Shared.h
//  MMLDemo_SourceCode
//
//  Created by Baidu Co.Ltd. on 2020/3/17.
//  Copyright © 2020 Baidu. All rights reserved.
//

#import "ViewController.h"

///mml  header
#import <MML/MMLPaddleConfig.h>
#import <MML/MMLMachineConfig.h>
#import <MML/MMLMachineService.h>
#import <MML/MMLDataProcess.h>
#import <MML/MMLLogger.h>
#import <MML/MML.h>

NS_ASSUME_NONNULL_BEGIN

static const int kMMLInputBatch      = 1;
static const int kMMLInputChannel    = 3;
static const int kMMLInputHeight     = 224;
static const int kMMLInputWidth      = 224;

@interface ViewController(MMLCore_OC)

- (MMLBaseMachine *)loadMMLWithModelDir_GPU:(NSString *)modelDir;
- (MMLBaseMachine *)loadMMLWithModelDir_CPU:(NSString *)modelDir;

@end

NS_ASSUME_NONNULL_END
