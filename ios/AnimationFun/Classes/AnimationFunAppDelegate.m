//
//  AnimationFunAppDelegate.m
//  AnimationFun
//
//  Created by david on 26/11/08.
//  Copyright n/a 2008. All rights reserved.
//

#import "AnimationFunAppDelegate.h"
#import "AnimationFunViewController.h"

@implementation AnimationFunAppDelegate

@synthesize window;
@synthesize viewController;


- (void)applicationDidFinishLaunching:(UIApplication *)application {    
    
    // Override point for customization after app launch    
	[viewController init];
	[viewController loadView];	
	
    [window addSubview:viewController.view];
    [window makeKeyAndVisible];
}


- (void)dealloc {
    [viewController release];
    [window release];
    [super dealloc];
}


@end
