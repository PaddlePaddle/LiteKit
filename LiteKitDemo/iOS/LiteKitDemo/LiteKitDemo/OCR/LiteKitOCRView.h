//
//  LiteKitOCRView.h
//  LiteKitOCRDemo
//
//  Created by Wang,Zhiyong on 2021/6/15.
//  Copyright © 2021 wangzhiyong04. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <LiteKitOCR/LiteKitOCR.h>

NS_ASSUME_NONNULL_BEGIN

@interface LiteKitOCRView : UIView
@property (nonatomic, strong) NSArray<LiteKitOCRResult *> *result;
@property (nonatomic, assign) CGSize presetSize;// 图像分辨率
@end

NS_ASSUME_NONNULL_END
