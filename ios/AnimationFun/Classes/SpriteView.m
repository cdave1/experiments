//
//  SpriteView.m
//  AnimationFun
//
//  Created by david on 26/11/08.
//  Copyright 2008 n/a. All rights reserved.
//

#import "SpriteView.h"


@implementation SpriteView

@synthesize spriteImage;

- (id)init 
{
	NSString *a = [[NSBundle mainBundle] pathForResource:@"marker" ofType:@"png"];
	self.spriteImage = [[UIImage alloc] initWithContentsOfFile:a];
	CGRect frame = CGRectMake(0, 0, self.spriteImage.size.width, self.spriteImage.size.height);
	
	if (self = [self initWithFrame:frame]) {	
		self.opaque = NO;
	}
	return self;
}

- (void)dealloc
{
	[spriteImage dealloc];
	[super dealloc];
}


- (void)drawRect:(CGRect)rect 
{
	[self.spriteImage drawAtPoint:CGPointMake(0.0f, 0.0f)];
}


@end
