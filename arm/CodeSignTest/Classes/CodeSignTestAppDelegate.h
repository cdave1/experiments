//
//  CodeSignTestAppDelegate.h
//  CodeSignTest
//
//  Created by david on 15/09/09.
//  Copyright n/a 2009. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "mach-o/arch.h"
#include "vfptest.h"
#include "neontest.h"





@interface CodeSignTestAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@end

