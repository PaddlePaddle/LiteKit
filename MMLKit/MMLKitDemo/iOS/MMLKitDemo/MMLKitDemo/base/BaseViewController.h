//
//  BaseViewController.h
//  MMLKitDemo
//
//  Created by Baidu on 2020/7/2.
//  Copyright © 2020 Baidu Co.,Ltd. . All rights reserved.
//

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

@end

NS_ASSUME_NONNULL_END
