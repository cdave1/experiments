//
//  CodeSignTestAppDelegate.m
//  CodeSignTest
//
//  Created by david on 15/09/09.
//  Copyright n/a 2009. All rights reserved.
//

#import "CodeSignTestAppDelegate.h"
#import <sys/utsname.h>

@implementation CodeSignTestAppDelegate

@synthesize window;
static char systemName[256];
- (void) applicationDidFinishLaunching:(UIApplication *)application
{
	struct utsname u;
	
	uname(&u);
	snprintf(systemName, 256, u.machine);
	
	if (0 == strcmp(systemName, "iPhone2,1"))
	{
		TestNeon();
	}
	else
	{
		TestVfp();
	}
	
	
	
	// Check the architecture. if arm7, call a function from arm7 static lib
	// otherwise call from arm6 static lib.
	/*
	NXArchInfo *archInfo = (NXArchInfo *)NXGetLocalArchInfo();
	
	if (strcmp("armv6", archInfo->name) == 0)
	{
		ArmTest::Test();
	}
	else if (strcmp("armv7", archInfo->name) == 0)
	{
		printf("Doing some arm7 stuff....");
		ArmTest::Test();
	}
	 */
	//NeonTest::ARM7_test();
}

- (void) applicationWillResignActive:(UIApplication *)application
{
}

- (void) applicationDidBecomeActive:(UIApplication *)application
{
}

- (void)applicationWillTerminate:(UIApplication *)application
{
}

- (void) dealloc
{
	[window release];
	
	[super dealloc];
}

@end
