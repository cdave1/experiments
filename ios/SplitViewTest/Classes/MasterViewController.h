//
//  MasterViewController.h
//  SplitViewTest
//
//  Created by David Petrie on 28/01/10.
//  Copyright Hackdirt Ltd 2010. All rights reserved.
//

#import <UIKit/UIKit.h>

@class DetailViewController;

@interface MasterViewController : UITableViewController {
    DetailViewController *detailViewController;
}

@property (nonatomic, retain) IBOutlet DetailViewController *detailViewController;

@end
