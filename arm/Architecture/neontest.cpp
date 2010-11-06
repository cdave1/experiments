/*
 *  neontest.cpp
 *  ArmTest
 *
 *  Created by david on 24/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "neontest.h"


void TestNeon()
{
	printf("I'm arm7 code.\n");
}
/*
void NEON_Matrix4Mul(const float* a, const float* b, float* output )
{
	_asm volatile 
	(
	 // Store A & B leaving room for q4-q7, which should be preserved
	 "vldmia %1, { q0-q3 } \n\t"
	 "vldmia %2, { q8-q11 }\n\t"
	 
	 // result = first column of B x first row of A
	 "vmul.f32 q12, q8, d0[0]\n\t"
	 "vmul.f32 q13, q8, d2[0]\n\t"
	 "vmul.f32 q14, q8, d4[0]\n\t"
	 "vmul.f32 q15, q8, d6[0]\n\t"
	 
	 // result += second column of B x second row of A
	 "vmla.f32 q12, q9, d0[1]\n\t"
	 "vmla.f32 q13, q9, d2[1]\n\t"
	 "vmla.f32 q14, q9, d4[1]\n\t"
	 "vmla.f32 q15, q9, d6[1]\n\t"
	 
	 // result += third column of B x third row of A
	 "vmla.f32 q12, q10, d1[0]\n\t"
	 "vmla.f32 q13, q10, d3[0]\n\t"
	 "vmla.f32 q14, q10, d5[0]\n\t"
	 "vmla.f32 q15, q10, d7[0]\n\t"
	 
	 // result += last column of B x last row of A
	 "vmla.f32 q12, q11, d1[1]\n\t"
	 "vmla.f32 q13, q11, d3[1]\n\t"
	 "vmla.f32 q14, q11, d5[1]\n\t"
	 "vmla.f32 q15, q11, d7[1]\n\t"
	 
	 // output = result registers
	 "vstmia %0, { q12-q15 }"
	 : // no output
	 : "r" (output), "r" (a), "r" (b) 	// input - note *value* of pointer doesn't change
	 : "memory", "q0", "q1", "q2", "q3", "q8", "q9", "q10", "q11", "q12", "q13", "q14", "q15" //clobber
	 );
}*/