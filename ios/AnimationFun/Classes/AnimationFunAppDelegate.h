//
//  AnimationFunAppDelegate.h
//  AnimationFun
//
//  Created by david on 26/11/08.
//  Copyright n/a 2008. All rights reserved.
//

#import <UIKit/UIKit.h>

@class AnimationFunViewController;

@interface AnimationFunAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    AnimationFunViewController *viewController;
}
@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet AnimationFunViewController *viewController;

@end

