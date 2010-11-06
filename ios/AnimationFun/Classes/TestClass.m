//
//  TestClass.m
//  AnimationFun
//
//  Created by david on 8/01/09.
//  Copyright 2009 n/a. All rights reserved.
//

#import "TestClass.h"


@implementation TestClass
@synthesize point, name;

- (id)initWithCoder:(NSCoder *)coder
{
	if ((self = [super init]) != nil) {
		self.point = CGPointMake([coder decodeFloatForKey:@"point.x"], [coder decodeFloatForKey:@"point.y"]);
		self.name = [coder decodeObjectForKey:@"name"];
	}
	return self;
}


- (void)encodeWithCoder:(NSCoder *)coder
{
	[coder encodeFloat:self.point.x forKey:@"point.x"];
	[coder encodeFloat:self.point.y forKey:@"point.y"];
	[coder encodeObject:name forKey:@"name"];
}
@end
