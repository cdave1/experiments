//
//  ParticleFunAppDelegate.m
//  ParticleFun
//
//  Created by david on 26/02/09.
//  Copyright n/a 2009. All rights reserved.
//

#import "ParticleFunAppDelegate.h"
#import "EAGLView.h"

@implementation ParticleFunAppDelegate

@synthesize window;
@synthesize glView;

- (void)applicationDidFinishLaunching:(UIApplication *)application {
    
	glView.animationInterval = 1.0 / 60.0;
	[glView startAnimation];
}


- (void)applicationWillResignActive:(UIApplication *)application {
	glView.animationInterval = 1.0 / 5.0;
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
	glView.animationInterval = 1.0 / 60.0;
}


- (void)dealloc {
	[window release];
	[glView release];
	[super dealloc];
}

@end
