//
//  MainView.h
//  AnimationFun
//
//  Created by david on 26/11/08.
//  Copyright 2008 n/a. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import "SpriteView.h"

@interface MainView : UIView 
{
	CALayer* theLayer;
	SpriteView *sprite;
}

@property(retain) CALayer* theLayer;
@property(retain) SpriteView *sprite;

@end


@interface SpriteTransformDelegate : NSObject
{
	MainView* parentView;
	SpriteView* sprite;
	CALayer* layer;
}
@property(retain) SpriteView* sprite;
@property(retain) MainView* parentView;
@property(retain) CALayer* layer;
- (void)animationDidStop:(CAAnimation *)theAnimation finished:(BOOL)flag;
@end
