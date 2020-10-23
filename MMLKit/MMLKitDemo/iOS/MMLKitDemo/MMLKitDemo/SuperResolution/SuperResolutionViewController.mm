//
//  SuperResolutionViewController.m
//  MMLKitDemo
//
//  Created by Baidu on 2020/7/13.
//  Copyright © 2020 Baidu Co.,Ltd. . All rights reserved.
//

#import "SuperResolutionViewController.h"
#import <MMLAIVideoSuperResolution/MMLAIVideoSuperResolution.h>

@interface SuperResolutionViewController ()
@property (nonatomic, strong) UIImage *image;
@property (strong, nonatomic) UILabel *timeCost;
@property (nonatomic, strong) UIImageView *originView;
@property (nonatomic, strong) UIImageView *superResolutionView;
@property (nonatomic, strong) MMLVideoSuperResolutionor *superVideo;
@end

@implementation SuperResolutionViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    [self initData];
    [self initViews];
}

- (void)initData {
    self.image = [UIImage imageNamed:@"test-SuperResolution.jpeg"];
    NSString *modelPath = [[[NSBundle mainBundle] bundlePath] stringByAppendingString:@"/SuperResolution.bundle"];
    MMLVideoSuperResolutionConfig *config = [[MMLVideoSuperResolutionConfig alloc] init];
    config.modelDir = modelPath;
    NSError *error = nil;
    MMLVideoSuperResolutionor *sVideo = [MMLVideoSuperResolutionor createInstanceWithConfig:config error:&error];
    if (error) {
        NSLog(@"出错1 ==== > %@", error);
        return;
    }
    self.superVideo = sVideo;
}

- (void)initViews {
    UIBarButtonItem *predict = [[UIBarButtonItem alloc] initWithTitle:@"预测"
                                                                style:UIBarButtonItemStylePlain
                                                               target:self
                                                               action:@selector(btnClickedAction:)];
    self.navigationItem.rightBarButtonItems = @[predict];
    
    self.originView = [[UIImageView alloc] initWithFrame:CGRectMake(10, [self naviBarHeight] + 20, 358, 210)];
    self.originView.contentMode = UIViewContentModeScaleAspectFit;
    self.originView.backgroundColor = [UIColor blueColor];
    self.originView.image = self.image;
    [self.view addSubview:self.originView];
    
    self.superResolutionView = [[UIImageView alloc] initWithFrame:CGRectMake(10, CGRectGetMaxY(self.originView.frame) + 20, 358, 210)];
    self.superResolutionView.contentMode = UIViewContentModeScaleAspectFit;
    self.superResolutionView.backgroundColor = [UIColor greenColor];
    [self.view addSubview:self.superResolutionView];
    
    UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(12, [self naviBarHeight] + 8, 100, 30)];
    label.textColor = [UIColor redColor];
    [self.view addSubview:label];
    self.timeCost = label;
}

- (void)btnClickedAction:(id)sender {
    NSError *error = nil;
    NSTimeInterval begin = [[NSDate date] timeIntervalSince1970];
    UIImage *newImg = [self.superVideo superResolutionWithUIImage:self.image scale:1.0 error:&error];
    NSTimeInterval end = [[NSDate date] timeIntervalSince1970];
    if (error) {
        NSLog(@"出错2 ==== > %@", error);
        return;
    }
    self.superResolutionView.image = newImg;
    self.timeCost.text = [NSString stringWithFormat:@"%.2fms", (end - begin) * 1000];
}

#pragma mark - internal
//导航栏高度+状态栏高度
- (float)naviBarHeight {
    return self.navigationController.navigationBar.frame.size.height + [[UIApplication sharedApplication] statusBarFrame].size.height;
}

@end
