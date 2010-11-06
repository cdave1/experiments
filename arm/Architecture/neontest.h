/*
 *  neontest.h
 *  ArmTest
 *
 *  Created by david on 24/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef NEON_TEST_H
#define NEON_TEST_H	

#include <stdio.h>
#ifdef __APPLE__
#include <TargetConditionals.h>
#include "arm/arch.h"
#endif



extern void TestNeon();
	
extern void NEON_Matrix4Mul(const float* a, const float* b, float* output );


#endif