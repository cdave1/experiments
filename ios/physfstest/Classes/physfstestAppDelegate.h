//
//  physfstestAppDelegate.h
//  physfstest
//
//  Created by David Petrie on 10/03/11.
//  Copyright 2011 n/a. All rights reserved.
//

#import <UIKit/UIKit.h>

@class physfstestViewController;

@interface physfstestAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    physfstestViewController *viewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet physfstestViewController *viewController;

@end

