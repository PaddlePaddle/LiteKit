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

#import <UIKit/UIKit.h>
#import "BaseDetectView.h"
#import <AVFoundation/AVFoundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol BaseViewControllerProtocol <NSObject>

/**
 * 需要子类覆写方法
 */
@required
/**
 * 预测结果展示view
*/
- (BaseDetectView*)inferenceContentView;
/**
 * 切换到使用图片预测模式
*/
- (void)switchImageModelAction:(id)sender;

@end

@interface BaseViewController : UIViewController<BaseViewControllerProtocol,AVCaptureVideoDataOutputSampleBufferDelegate>

@property (strong, nonatomic) BaseDetectView *detectView;

/**
 * 导航栏高度+状态栏高度
 */
- (float)naviBarHeight;
- (CGSize)captureSessionPresetSize;

- (BOOL)isNeedCamera;

//横竖屏切换时：显示相机流的layer更新frame
- (void)updateViewLayout:(CGSize)screenSize;

@end

NS_ASSUME_NONNULL_END
