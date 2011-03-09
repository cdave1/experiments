//
//  physfstestAppDelegate.m
//  physfstest
//
//  Created by David Petrie on 10/03/11.
//  Copyright 2011 n/a. All rights reserved.
//

#import "physfstestAppDelegate.h"
#import "physfstestViewController.h"

#include <stdio.h>
#include <stdlib.h>
#include "physfs.h"

@implementation physfstestAppDelegate

@synthesize window;
@synthesize viewController;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    PHYSFS_init(NULL);
	PHYSFS_addToSearchPath([[NSString stringWithFormat:@"%@/myzip2.zip", [[NSBundle mainBundle] bundlePath]] UTF8String], 1);
	if (PHYSFS_exists("folder/test.txt"))
	{
		PHYSFS_file* myfile = PHYSFS_openRead("folder/test.txt");
        char * contents;
        contents = (char *)calloc(1, 1+PHYSFS_fileLength(myfile));
        int length_read = PHYSFS_read(myfile, contents, 1, PHYSFS_fileLength(myfile));
		printf("%s (%d)\n", contents, length_read);
        PHYSFS_close(myfile);
	}
    PHYSFS_deinit();
    printf("done\n");
    
    self.window.rootViewController = self.viewController;
    return YES;
}

- (void)dealloc
{
    [viewController release];
    [window release];
    [super dealloc];
}

@end
