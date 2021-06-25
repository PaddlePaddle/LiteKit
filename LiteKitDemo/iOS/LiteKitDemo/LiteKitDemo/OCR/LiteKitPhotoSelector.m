//
//  LiteKitPhotoSelector.m
//  LiteKitOCRDemo
//
//  Created by Wang,Zhiyong on 2021/6/16.
//  Copyright © 2021 wangzhiyong04. All rights reserved.
//

#import "LiteKitPhotoSelector.h"

@interface LiteKitPhotoSelector() <UINavigationControllerDelegate,UIImagePickerControllerDelegate>
@property (nonatomic, copy) LiteKitImageSelectBlock selectedBlock;

@end

@implementation LiteKitPhotoSelector

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code
}
*/


- (void)selectPhoto:(UIViewController *)vc completion:(LiteKitImageSelectBlock)completionBlock {
    self.selectedBlock = completionBlock;
    
    UIImagePickerController *imagePickerController = [[UIImagePickerController alloc]init];
    imagePickerController.allowsEditing = YES;
    imagePickerController.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    imagePickerController.delegate = self;
    [vc presentViewController:imagePickerController animated:YES completion:nil];
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info {
    NSLog(@"didFinishPickingMediaWithInfo");
    [picker dismissViewControllerAnimated:YES completion:nil];
    UIImage *image = [info objectForKey:UIImagePickerControllerOriginalImage];
    if (self.selectedBlock) {
        self.selectedBlock(image);
    }
}

@end
