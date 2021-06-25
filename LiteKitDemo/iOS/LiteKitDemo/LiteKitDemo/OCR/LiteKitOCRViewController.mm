//
//  ViewController.m
//  LiteKitOCRDemo
//
//  Created by Wang,Zhiyong on 2017/11/30.
//  Copyright © 2017年 wangzhiyong04. All rights reserved.
//

#import "LiteKitOCRViewController.h"
#import <LiteKitOCR/LiteKitOCR.h>
#import "LiteKitOCRView.h"
#import "LiteKitPhotoSelector.h"

@interface LiteKitOCRViewController ()
@property (nonatomic, strong) LiteKitOCRView *ocrView;
@property (nonatomic, strong) LiteKitOCRPredictor *predictor;
@property (nonatomic, strong) UIImage *inputImage;
@property (nonatomic, strong) UIImageView *imageView;
@property (nonatomic, strong) LiteKitPhotoSelector *imageSelector;
@property (nonatomic, strong) UITextView *stateView;
@end


@implementation LiteKitOCRViewController

static NSInteger LoadButtonTag = 1000;
static CGFloat Gap = 15.0f;

#pragma mark - viewDidLoad
- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    
    
    UIButton *loadBtn = [self addButton:@"加载模型" action:@selector(loadButtonAction:) tag:1000];
    [self addButton:@"执行预测" action:@selector(runButtonAction:) tag:1001];
    [self addButton:@"选择照片" action:@selector(selectButtonAction:) tag:1002];
    
    
    CGRect stateFrame = CGRectMake(CGRectGetMinX(loadBtn.frame), CGRectGetMaxY(loadBtn.frame)+Gap,
                                   CGRectGetWidth(loadBtn.frame)*3+Gap*2, 100);
    self.stateView = [[UITextView alloc] initWithFrame:stateFrame];
    [self.stateView setEditable:false];
    [self.view addSubview:self.stateView];
    [self updateState:@"idle"];
    
    
    UIImageView *imageView = [[UIImageView alloc] init];
    [self.view addSubview:imageView];
    self.ocrView = [[LiteKitOCRView alloc] init];
    self.ocrView.backgroundColor = [UIColor clearColor];
    [imageView addSubview:self.ocrView];
    UIImage *input = [UIImage imageNamed:@"test_OCR_0.jpg"];
    [self setImage:input toImageView:imageView];
    
}

#pragma mark - Button Actions
#pragma mark loadButton
- (void)loadButtonAction:(id)sender {
    if (self.predictor == nil) { // to load
        NSError *error = nil;
        LiteKitOCRPredictor *predictor = [LiteKitOCRPredictor createOCRPredictorWithError:&error];
        
        if (!error) {
            self.predictor = predictor;
            [self updateState:@"load model successed"];
        } else {
            [self updateState:[NSString stringWithFormat:@"load model fail. %@", error]];
        }
    } else {
        self.ocrView.result = nil;
        self.predictor = nil;
        [self updateState:@"release successed"];
    }
    
    UIButton *loadBtn = [self.view viewWithTag:LoadButtonTag];
    if (self.predictor == nil) { // loaded
        [loadBtn setTitle:@"加载模型" forState:UIControlStateNormal];
        [loadBtn setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    } else { // update
        [loadBtn setTitle:@"释放模型" forState:UIControlStateNormal];
        [loadBtn setTitleColor:[UIColor darkGrayColor] forState:UIControlStateNormal];
    }
}
#pragma mark runButton
- (void)runButtonAction:(id)sender {
    NSError *error = nil;
    NSArray<LiteKitOCRResult *> *result = [self.predictor recognizeWithImage:self.inputImage error:&error];
    
    if (!error && result) {
        self.ocrView.result = result;
        [self updateState:[NSString stringWithFormat:@"run model successed. %@", [self currentRresult]]];
    } else {
        [self updateState:[NSString stringWithFormat:@"run model fail. %@", error]];
    }
}
#pragma mark selectButton
- (void)selectButtonAction:(id)sender {
    self.imageSelector = [[LiteKitPhotoSelector alloc] init];
    [self.imageSelector selectPhoto:self completion:^(UIImage * _Nonnull selectedImage) {
        [self setImage:selectedImage toImageView:self.imageView];
    }];
}


#pragma mark - Other
- (UIButton *)addButton:(NSString *)name action:(SEL)selector tag:(NSInteger)tag {
    CGFloat screenWidth = [[UIScreen mainScreen] bounds].size.width;
    CGFloat width = (screenWidth-Gap*4)/3;
    
    UIButton *preButton = [self.view viewWithTag:tag-1];
    CGFloat x = Gap;
    if (preButton != nil) {
        x = CGRectGetMaxX(preButton.frame) + Gap;
    }
    CGFloat naviBarHeight = self.navigationController.navigationBar.frame.size.height;
    CGFloat statusBarHeight = [[UIApplication sharedApplication] statusBarFrame].size.height;
    CGFloat y = statusBarHeight+naviBarHeight + Gap;
    CGRect frame = CGRectMake(x, y, width, 50);
    
    UIButton *aButton = [[UIButton alloc] initWithFrame:frame];
    [aButton setTag:tag];
    [aButton setTitle:name forState:UIControlStateNormal];
    [aButton setBackgroundImage:[self imageWithColor:[UIColor lightGrayColor]] forState:UIControlStateNormal];
    [aButton addTarget:self action:selector forControlEvents:UIControlEventTouchUpInside];
    [self.view addSubview:aButton];
    
    return aButton;
}

// 返回一张纯色图片
- (UIImage *)imageWithColor:(UIColor *)color {
    CGRect rect = CGRectMake(0, 0, 1, 1);
    // 开启位图上下文
    UIGraphicsBeginImageContext(rect.size);
    // 获取位图上下文
    CGContextRef context = UIGraphicsGetCurrentContext();
    // 使用color演示填充上下文
    CGContextSetFillColorWithColor(context, [color CGColor]);
    // 渲染上下文
    CGContextFillRect(context, rect);
    // 从上下文中获取图片
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    // 结束上下文
    UIGraphicsEndImageContext();
    
    return image;
}

- (void)updateState:(NSString *)state {
    NSLog(@"%@", state);
    [self.stateView setText:[NSString stringWithFormat:@"STATUS: %@", state]];
}

- (NSString *)currentRresult {
    if (self.ocrView.result == nil) {
        return @"nil";
    } else {
        NSMutableString *string = [[NSMutableString alloc] initWithString:@"\n"];
        for (NSInteger index=0; index<self.ocrView.result.count; index++) {
            LiteKitOCRResult *result = [self.ocrView.result objectAtIndex:index];
            [string appendFormat:@"\n%ld. %@", index+1, result.label];
        }
        return [string copy];
    }
    return @"";
}

- (void)setImage:(UIImage *)image toImageView:(UIImageView *)imageView {
    self.inputImage = image;
    self.imageView = imageView;
    self.imageView.image = image;
    
    CGFloat screenWidth = [[UIScreen mainScreen] bounds].size.width;
    CGFloat maxWidth = screenWidth-2*Gap;
    
    CGFloat scaleW = MIN(image.size.width, maxWidth)/image.size.width;
    CGFloat scaleH = MIN(image.size.height, maxWidth)/image.size.height;
    CGFloat scale = MIN(scaleW, scaleH);
    imageView.frame = CGRectMake(Gap, CGRectGetMaxY(self.stateView.frame)+Gap, image.size.width*scale, image.size.height*scale);
    
    self.ocrView.frame = imageView.bounds;
    self.ocrView.result = nil;
    self.ocrView.presetSize = CGSizeMake(image.size.width, image.size.height);
    
    [self updateState:@"idle"];
}

@end
