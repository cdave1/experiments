//
//  MainView.m
//  AnimationFun
//
//  Created by david on 26/11/08.
//  Copyright 2008 n/a. All rights reserved.
//

#import "MainView.h"
#import "TestClass.h"
#import <stdlib.h>

@implementation SpriteTransformDelegate

@synthesize parentView;
@synthesize layer;
@synthesize sprite;
- (void)animationDidStop:(CAAnimation *)theAnimation finished:(BOOL)flag
{
	//[CATransaction begin];
	CABasicAnimation *transform = [CABasicAnimation animationWithKeyPath:@"transform"];
	transform.duration = 0.75f;
	transform.delegate = self.parentView;
	transform.toValue = [NSValue valueWithCATransform3D:CATransform3DIdentity];
	transform.removedOnCompletion = NO; 
	[self.layer addAnimation:transform forKey:@"myTransformKey"];
}
@end


@implementation MainView

@synthesize theLayer, sprite;

- (id)initWithFrame:(CGRect)frame 
{	
	if (self = [super initWithFrame:frame]) 
	{
		SpriteView *s = [[SpriteView alloc] init];
		self.sprite = s;
		[s release]; 
		
		// Try and load the previously saved position:
		NSFileManager *fileManager = [NSFileManager defaultManager];
		NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
		NSString *documentsDirectory = [paths objectAtIndex:0];
		NSString *path = [documentsDirectory stringByAppendingPathComponent:@"datafile.txt"];
		
		// Check if the file exists
		if ([fileManager fileExistsAtPath:path])
		{
			NSData* data = [NSKeyedUnarchiver unarchiveObjectWithFile:path];
			TestClass *tc = [NSKeyedUnarchiver unarchiveObjectWithData:data];
			s.center = tc.point;
		} 
		else 
		{
			s.center = self.center;
		}
		
		[self addSubview:s];
	}
	return self;
}


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch *touch = [touches anyObject];	
	CGPoint point = [touch locationInView:self];
	sprite.center = point;
	
	// Also want to scale the image up
	CABasicAnimation* basic = [CABasicAnimation animationWithKeyPath:@"transform"];
	basic.duration = 0.1f;
	CGAffineTransform transform = CGAffineTransformMakeScale(2.5, 2.5);
	basic.removedOnCompletion = NO;
	basic.toValue =  [NSValue valueWithCGAffineTransform:transform]; 	
	[sprite.layer addAnimation:basic forKey:@"transform"];
	sprite.transform = transform;
}


- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch *touch = [touches anyObject];	
	CGPoint point = [touch locationInView:self];
	sprite.center = point;
}


- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
	UITouch *touch = [touches anyObject];	
	CGPoint point = [touch locationInView:self];
	
	// Save the point
	// Create an instance of TestClass and then save it to the datafile using
	// a keyed archiver:
	TestClass* tc = [[TestClass alloc] init];
	tc.point = point;
	tc.name = @"foolish";
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *path = [documentsDirectory stringByAppendingPathComponent:@"datafile.txt"];
	//[NSArchiver archiveRootObject:tc toFile:path];
	//[NSKeyedArchiver archiveRootObject:tc toFile:path];	
	//[NSKeyedArchiver archiveRootObject:tc.b toFile:path];	
	NSData *data = [NSKeyedArchiver archivedDataWithRootObject:tc];
	[NSKeyedArchiver archiveRootObject:data toFile:path];
	
	
	
	CALayer *spriteLayer = sprite.layer;
	SpriteTransformDelegate* del = [[SpriteTransformDelegate alloc] init];
	del.parentView = self;
	del.layer = spriteLayer;
	del.sprite = sprite;
	
	CAKeyframeAnimation *move = [CAKeyframeAnimation animationWithKeyPath:@"position"];
	CGMutablePathRef thePath = CGPathCreateMutable();
	move.delegate = self; //del;
	move.removedOnCompletion = NO;
	CGPathMoveToPoint(thePath, NULL, sprite.center.x, sprite.center.y);
	CGPathAddLineToPoint(thePath, NULL, 0.0f, 0.0f);
	CGPathAddLineToPoint(thePath, NULL, point.x, point.y);
	
	move.repeatCount = 1;
	move.path = thePath;
	move.duration = 0.5f;
	
	[spriteLayer addAnimation:move forKey:@"movement"];
	sprite.center = point;
	
	// The UIView stuff is good for very simply animations, but I don't 
	// think it offers enough control for doing complex animations - path related
	// stuff appears to be quite hard. However, I think it's ideal for transitions
	// of simple view menus and images.
	/*
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:0.10];
	[UIView setAnimationDelegate:self];	
	sprite.center = point;
	[UIView commitAnimations];
	
	[UIView beginAnimations:nil context:NULL];
	[UIView setAnimationDuration:0.10];
	[UIView setAnimationDelegate:self];
	//[UIView setAnimationDidStopSelector:@selector(growAnimationDidStop:finished:context:)];
	CGAffineTransform transform = CGAffineTransformMakeScale(1.0, 1.0);
	sprite.transform = transform;
	[UIView commitAnimations];*/
}

- (void)animationDidStop:(CAAnimation *)theAnimation finished:(BOOL)flag 
{
	CAAnimation *movement = [self.sprite.layer animationForKey:@"movement"];
	CAAnimation *myTransform = [self.sprite.layer animationForKey:@"myTransformKey"];
	
	if (theAnimation == movement)
	{
		sprite.transform = CGAffineTransformIdentity;
		CABasicAnimation *transform = [CABasicAnimation animationWithKeyPath:@"transform"];
		transform.duration = 0.1f;
		transform.delegate = self;
		CGAffineTransform from = CGAffineTransformMakeScale(2.5f, 2.5f);
		CGAffineTransform to = CGAffineTransformIdentity;
		transform.fromValue = [NSValue valueWithCGAffineTransform:from];
		transform.toValue = [NSValue valueWithCGAffineTransform:to];
		transform.removedOnCompletion = NO; 
		[self.sprite.layer addAnimation:transform forKey:@"myTransformKey"];
	}
}

@end
