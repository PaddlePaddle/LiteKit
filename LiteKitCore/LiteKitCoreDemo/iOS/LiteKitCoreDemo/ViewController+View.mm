//
//  ViewController+View.m
//  LiteKitCoreDemo
//
//  Created by Wang,Zhiyong on 2021/2/24.
//  Copyright © 2021 Baidu. All rights reserved.
//

#import "ViewController+View.h"


@implementation ViewController(View)
typedef NS_ENUM(NSInteger, DemoButtonType) {
    DemoButtonType_OC_CPU = 0,
    DemoButtonType_OC_GPU = 1,
    DemoButtonType_CXX_CPU = 2,
    DemoButtonType_CXX_GPU = 3
};


- (void)createViews {
    [self addButton:DemoButtonType_OC_CPU title:@"OC_CPU"];
    [self addButton:DemoButtonType_OC_GPU title:@"OC_GPU"];
    [self addButton:DemoButtonType_CXX_CPU title:@"CXX_CPU"];
    [self addButton:DemoButtonType_CXX_GPU title:@"CXX_GPU"];
    
}

- (void)addButton:(NSInteger)index title:(NSString *)title {
    int height = 50;
    UIButton *button = [[UIButton alloc] initWithFrame:CGRectMake(100, 50+(height+10)*index, 100, height)];
    [button setBackgroundColor:[UIColor whiteColor]];
    [button setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [button setTitle:title forState:UIControlStateNormal];
    
    
    [self addActionToButton:button actionType:(DemoButtonType)index];
    
    [self.view addSubview:button];
}

- (void)addActionToButton:(UIButton *)button actionType:(DemoButtonType)index {
    switch (index) {
        case DemoButtonType_OC_CPU: {
            [button addTarget:self action:@selector(LiteKit_OC_Demo_CPU) forControlEvents:UIControlEventTouchUpInside];
            break;
        }
        case DemoButtonType_OC_GPU: {
            [button addTarget:self action:@selector(LiteKit_OC_Demo_GPU) forControlEvents:UIControlEventTouchUpInside];
            break;
        }
        case DemoButtonType_CXX_CPU: {
            [button addTarget:self action:@selector(LiteKit_CPP_Demo_CPU) forControlEvents:UIControlEventTouchUpInside];
            break;
        }
        case DemoButtonType_CXX_GPU: {
            [button addTarget:self action:@selector(LiteKit_CPP_Demo_GPU) forControlEvents:UIControlEventTouchUpInside];
            break;
        }
        default:
            break;
    }
}


- (void)LiteKit_OC_Demo_CPU {
    NSBundle *testBundle = [NSBundle mainBundle];
    NSString *filePath = @"LiteKitCoreDemo.bundle";
    NSString *modelDir = [testBundle pathForResource:filePath ofType:nil];
    
    [self LiteKit_OC_Demo_CPU:modelDir];
}

- (void)LiteKit_OC_Demo_GPU {
    NSBundle *testBundle = [NSBundle mainBundle];
    NSString *filePath = @"LiteKitCoreDemo.bundle";
    NSString *modelDir = [testBundle pathForResource:filePath ofType:nil];
    
    [self LiteKit_OC_Demo_GPU:modelDir];
}

- (void)LiteKit_CPP_Demo_CPU {
    NSBundle *testBundle = [NSBundle mainBundle];
    NSString *filePath = @"LiteKitCoreDemo.bundle";
    NSString *modelDir = [testBundle pathForResource:filePath ofType:nil];
    
    [self LiteKit_CPP_Demo_CPU:modelDir];
}

- (void)LiteKit_CPP_Demo_GPU {
    NSBundle *testBundle = [NSBundle mainBundle];
    NSString *filePath = @"LiteKitCoreDemo.bundle";
    NSString *modelDir = [testBundle pathForResource:filePath ofType:nil];
    
    [self LiteKit_CPP_Demo_GPU:modelDir];
}

@end
