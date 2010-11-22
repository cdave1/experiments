/*
 *  mytest.h
 *  ArmTest
 *
 *  Created by David Petrie on 13/12/09.
 *  Copyright 2009 Hackdirt Ltd. All rights reserved.
 *
 */

#ifndef MY_TEST_H
#define MY_TEST_H


#include <stdio.h>
#ifdef __APPLE__
#include <TargetConditionals.h>
#include "arm/arch.h"
#endif

class MyTest
{
public:
	static void TestMe();
};


#endif