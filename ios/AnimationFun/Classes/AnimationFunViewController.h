//
//  AnimationFunViewController.h
//  AnimationFun
//
//  Created by david on 26/11/08.
//  Copyright n/a 2008. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import "MainView.h"

@interface AnimationFunViewController : UIViewController {
	MainView* mainView;
}

@property(retain) MainView* mainView;

@end

