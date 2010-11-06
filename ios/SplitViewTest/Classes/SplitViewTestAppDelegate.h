//
//  SplitViewTestAppDelegate.h
//  SplitViewTest
//
//  Created by David Petrie on 28/01/10.
//  Copyright Hackdirt Ltd 2010. All rights reserved.
//

#import <UIKit/UIKit.h>


@class MasterViewController;
@class DetailViewController;

@interface SplitViewTestAppDelegate : NSObject <UIApplicationDelegate> {
    
    UIWindow *window;
    
    UISplitViewController *splitViewController;
    
    MasterViewController *masterViewController;
    DetailViewController *detailViewController;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic,retain) IBOutlet UISplitViewController *splitViewController;
@property (nonatomic,retain) IBOutlet MasterViewController *masterViewController;
@property (nonatomic,retain) IBOutlet DetailViewController *detailViewController;

@end
