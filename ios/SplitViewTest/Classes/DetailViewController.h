//
//  DetailViewController.h
//  SplitViewTest
//
//  Created by David Petrie on 28/01/10.
//  Copyright Hackdirt Ltd 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface DetailViewController : UIViewController <UIPopoverControllerDelegate, UISplitViewControllerDelegate> {
    
    UIPopoverController *popoverController;
    UINavigationBar *navigationBar;
    
    id detailItem;
}

@property (nonatomic, retain) UIPopoverController *popoverController;
@property (nonatomic, retain) IBOutlet UINavigationBar *navigationBar;

@property (nonatomic, retain) id detailItem;

@end
