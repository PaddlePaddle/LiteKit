//
//  HandGestureRecognizeView.h
//  MMLKitDemo
//
//  Created by Baidu on 2020/7/2.
//  Copyright © 2020 Baidu Co.,Ltd. . All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BaseDetectView.h"
#import <MMLHandGestureDetection/MMLHandGestureDetector.h>

NS_ASSUME_NONNULL_BEGIN

@interface HandGestureRecognizeView : BaseDetectView

- (void)updateResultData:(MMLHandGestureDetectResult *)resultData;

@end

NS_ASSUME_NONNULL_END
