//
//  ParticleFunAppDelegate.h
//  ParticleFun
//
//  Created by david on 26/02/09.
//  Copyright n/a 2009. All rights reserved.
//

#import <UIKit/UIKit.h>

@class EAGLView;

@interface ParticleFunAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    EAGLView *glView;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet EAGLView *glView;

@end

