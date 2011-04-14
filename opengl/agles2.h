/*
 
 Copyright (c) 2011 David Petrie
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
 */

#ifndef agl_ES_GLUE_H
#define agl_ES_GLUE_H

#include <stdio.h>
#include <assert.h>

#ifndef GL_QUADS
#define GL_QUADS 888
#endif

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>


#ifdef __cplusplus
extern "C" {
#endif
	
	extern void aglBegin(unsigned int prim);
    
    extern void aglTranslatef(float x, float y, float z);
	
	extern void aglVertex3f(float x, float y, float z);
	
	extern void aglVertex2f(float x, float y);
	
	extern void aglColor4f(float r, float g, float b, float a );
		
	extern void aglTexCoord2f(float s, float t );
	
	extern void aglEnd();
	
	extern void aglError(const char *source);

#ifdef aglES2
    
    extern void aglSetCamera(float left, float right, float bottom, float top, float zNear, float zFar);
    
#endif
    
	
#ifdef __cplusplus
}
#endif



#endif