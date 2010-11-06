//
//  TestClass.h
//  AnimationFun
//
//  Created by david on 8/01/09.
//  Copyright 2009 n/a. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface TestClass : NSObject <NSCoding> {
	CGPoint point;
	NSString* name;
}
@property(readwrite) CGPoint point;
@property(readwrite, retain) NSString* name;

@end
