//
//  LiteKitPhotoSelector.h
//  LiteKitOCRDemo
//
//  Created by Wang,Zhiyong on 2021/6/16.
//  Copyright © 2021 wangzhiyong04. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef void (^LiteKitImageSelectBlock)(UIImage *selectedImage);

@interface LiteKitPhotoSelector : UIView

- (void)selectPhoto:(UIViewController *)vc completion:(LiteKitImageSelectBlock)completionBlock;

@end

NS_ASSUME_NONNULL_END
