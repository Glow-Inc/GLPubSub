//
//  ViewController.m
//  GLPubSub Example
//
//  Created by Allen Hsu on 3/12/15.
//  Copyright (c) 2015 Glow, Inc. All rights reserved.
//

#import "ViewController.h"

NSString * const GLNotificationDidClickButton       = @"GLNotificationDidClickButton";
NSString * const GLNotificationFromNonMainThread    = @"GLNotificationFromNonMainThread";

@interface ViewController () <UITextFieldDelegate>

@property (weak, nonatomic) IBOutlet UITextView *console;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    __weak __typeof__(self) weakSelf = self;
    [self subscribe:UIApplicationWillEnterForegroundNotification handler:^(GLEvent *event) {
        __strong __typeof__(weakSelf) strongSelf = weakSelf;
        [strongSelf logToConsole:@"WillEnterForeground"];
    }];
    [self subscribe:UIApplicationDidEnterBackgroundNotification handler:^(GLEvent *event) {
        __strong __typeof__(weakSelf) strongSelf = weakSelf;
        [strongSelf logToConsole:@"DidEnterBackground"];
    }];
    [self subscribe:UIApplicationDidBecomeActiveNotification handler:^(GLEvent *event) {
        __strong __typeof__(weakSelf) strongSelf = weakSelf;
        [strongSelf logToConsole:@"DidBecomeActive"];
    }]; 
    [self subscribe:GLNotificationDidClickButton handler:^(GLEvent *event) {
        __strong __typeof__(weakSelf) strongSelf = weakSelf;
        [strongSelf logToConsole:@"DidClickButton"];
    }];
    [self subscribe:GLNotificationFromNonMainThread handler:^(GLEvent *event) {
        __strong __typeof__(weakSelf) strongSelf = weakSelf;
        [strongSelf logToConsole:@"FromNonMainThrad"];
    }];
    [self subscribe:UITextFieldTextDidChangeNotification selector:@selector(textDidChange)];
    
}

- (void)dealloc
{
    [self unsubscribeAll];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)textDidChange
{
    [self logToConsole:@"UITextFieldTextDidChange"];
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return NO;
}

- (IBAction)didClickButton:(id)sender {
    [self publish:GLNotificationDidClickButton];
}

- (IBAction)sendNonMainThreadEvent:(id)sender {
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
        [self publish:GLNotificationFromNonMainThread];
    });
}

- (void)logToConsole:(NSString *)log
{
    self.console.text = [self.console.text stringByAppendingFormat:@"%@\n", log];
}

@end
