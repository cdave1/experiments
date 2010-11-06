//
//  AnimationFunViewController.m
//  AnimationFun
//
//  Created by david on 26/11/08.
//  Copyright n/a 2008. All rights reserved.
//

#import "AnimationFunViewController.h"

@implementation AnimationFunViewController

@synthesize mainView;

- init
{
	self = [super init];
	if (self != nil)
	{
		mainView = [[MainView alloc] initWithFrame:[UIScreen mainScreen].bounds];
	}
	return self;
}


- (void)loadView 
{
	self.view = mainView;	
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

@end
