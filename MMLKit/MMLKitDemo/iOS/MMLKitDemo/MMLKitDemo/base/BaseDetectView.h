//
//  BaseDetectView.h
//  MMLKitDemo
//
//  Created by Baidu on 2020/7/2.
//  Copyright © 2020 Baidu Co.,Ltd. . All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface BaseDetectView : UIView

@property (nonatomic, assign) float offsetY; // 子控件布局的起始位置
@property (nonatomic, assign) CGSize presetSize;// 图像分辨率

@end

NS_ASSUME_NONNULL_END
