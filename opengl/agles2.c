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

#include "aglesGlue.h"
#include "strings.h"

#define aglES_GLUE_MAX_VERTICES 32768

const GLchar * vertexaglesShaderProgram = "\
attribute vec4 position;\
attribute vec4 color;\
varying vec4 colorVarying;\
\
uniform mat4 camera;\
void main()\
{\
    colorVarying = color;\
    gl_Position = (camera * position);\
}\n\
";


const GLchar * fragmentaglesShaderProgram = "\
varying lowp vec4 colorVarying;\
void main()\
{\
    gl_FragColor = colorVarying;\
}\n\
";

static float aglCameraMat[16];




GLuint aglCameraUniform;


enum {
    ATTRIB_VERTEX,
    ATTRIB_COLOR,
    NUM_ATTRIBUTES
};


typedef struct 
{
	float xyz[3];
	float st[2];
	GLubyte rgba[4];
} aglesVertex_t;


typedef struct 
{
	aglesVertex_t vertices[aglES_GLUE_MAX_VERTICES];
	short quadIndices[aglES_GLUE_MAX_VERTICES * 3 / 2];
	aglesVertex_t currVertex;
	unsigned int currIndex;
} aglesGlueArrays_t;

aglesGlueArrays_t aglesGlueArrays;

GLenum aglesCurrentPrimitive = GL_TRIANGLES;
bool aglesQuadIndicesInitted = false;
bool aglesShadersInitted = false;
GLuint aglesShaderProgram = 0;


GLuint aglCompileShader(const GLchar * source, const GLenum type)
{
    assert(aglesShaderProgram);
    assert(strlen(source) > 0);
    
    printf("%s\n", source);
    
    GLuint shader;
    GLint status;
    
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    
    GLint logLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(shader, logLength, &logLength, log);
        printf("Shader compile log:\n%s\n", log);
        free(log);
    }
    
    assert(status != 0);
    
    glAttachShader(aglesShaderProgram, shader);
    
    return shader;
}


GLvoid aglBegin(GLenum prim) 
{
	if (!aglesQuadIndicesInitted)
	{
		for (int i = 0; i < aglES_GLUE_MAX_VERTICES * 3 / 2; i += 6) 
		{
			int q = i / 6 * 4;
			aglesGlueArrays.quadIndices[i + 0] = q + 0;
			aglesGlueArrays.quadIndices[i + 1] = q + 1;
			aglesGlueArrays.quadIndices[i + 2] = q + 2;
			
			aglesGlueArrays.quadIndices[i + 3] = q + 0;
			aglesGlueArrays.quadIndices[i + 4] = q + 2;
			aglesGlueArrays.quadIndices[i + 5] = q + 3;
		}
		aglesQuadIndicesInitted = true;
	}
    
    if (!aglesShadersInitted)
    {
        /*
        aglSetCamera(-320.0f, 320.0f, -480.0f, 480.0f, -10000.0f, 10000.0f);
        
        // Compile vertex shaders...
        aglesShaderProgram = glCreateProgram();
        
        GLuint vertexShader = aglCompileShader(vertexaglesShaderProgram, GL_VERTEX_SHADER);
        GLuint fragmentShader = aglCompileShader(fragmentaglesShaderProgram, GL_FRAGMENT_SHADER);
        assert(vertexShader && fragmentShader);
        
        glBindAttribLocation(aglesShaderProgram, ATTRIB_VERTEX, "position");
        glBindAttribLocation(aglesShaderProgram, ATTRIB_COLOR, "color");
        
        glLinkProgram(aglesShaderProgram);
        
        // Get uniform locations
        //uniformIndices[ROT_MATRIX] = glGetUniformLocation(aglesShaderProgram, "rotation");
        //uniformIndices[CAMERA_MATRIX] = glGetUniformLocation(aglesShaderProgram, "camera");
        //uniformIndices[TRANSLATION_UNIFORM] = glGetUniformLocation(aglesShaderProgram, "translation");
        
        aglCameraUniform = glGetUniformLocation(aglesShaderProgram, "camera");
        
        // Release vertex and fragment shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        */
        aglesShadersInitted = true;
    }
    
    assert(aglesQuadIndicesInitted && aglesShadersInitted);
    
	aglesGlueArrays.currIndex = 0;
	aglesCurrentPrimitive = prim;
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glEnableVertexAttribArray(ATTRIB_COLOR);
}


GLvoid aglTranslatef(float x, float y, float z)
{
    //glTranslatef(x, y, z);
}


GLvoid aglVertex3f(float x, float y, float z) 
{
	if (aglesGlueArrays.currIndex >= aglES_GLUE_MAX_VERTICES)
	{
		return;
	}
	
	aglesGlueArrays.currVertex.xyz[0] = x;
	aglesGlueArrays.currVertex.xyz[1] = y;
	aglesGlueArrays.currVertex.xyz[2] = z;
	aglesGlueArrays.vertices[aglesGlueArrays.currIndex] = aglesGlueArrays.currVertex;
	aglesGlueArrays.currIndex++;
}


GLvoid aglVertex2f(float x, float y) 
{
	if (aglesGlueArrays.currIndex >= aglES_GLUE_MAX_VERTICES)
	{
		return;
	}
	
	aglesGlueArrays.currVertex.xyz[0] = x;
	aglesGlueArrays.currVertex.xyz[1] = y;
	aglesGlueArrays.currVertex.xyz[2] = 0.0f;
	aglesGlueArrays.vertices[aglesGlueArrays.currIndex] = aglesGlueArrays.currVertex;
	aglesGlueArrays.currIndex++;
}


GLvoid aglColor4ub(GLubyte r, GLubyte g, GLubyte b, GLubyte a) 
{
	aglesGlueArrays.currVertex.rgba[0] = r;
	aglesGlueArrays.currVertex.rgba[1] = g;
	aglesGlueArrays.currVertex.rgba[2] = b;
	aglesGlueArrays.currVertex.rgba[3] = a;
}


GLvoid aglColor4f(GLfloat r, GLfloat g, GLfloat b, GLfloat a) 
{
	aglesGlueArrays.currVertex.rgba[0] = (GLubyte) (r * 255);
	aglesGlueArrays.currVertex.rgba[1] = (GLubyte) (g * 255);
	aglesGlueArrays.currVertex.rgba[2] = (GLubyte) (b * 255);
	aglesGlueArrays.currVertex.rgba[3] = (GLubyte) (a * 255);
}


GLvoid aglTexCoord2f(GLfloat s, GLfloat t) 
{
	aglesGlueArrays.currVertex.st[0] = s;
	aglesGlueArrays.currVertex.st[1] = t;
}



void aglSetCamera(float left, float right, float bottom, float top, float zNear, float zFar)
{
    //memcpy(aglCameraMat, camera, sizeof(float) * 16);

    
    
    
    if (right != left)
    {
        aglCameraMat[ 0] = 2 / (right - left);
        aglCameraMat[ 1] = 0;
        aglCameraMat[ 2] = 0;
        aglCameraMat[ 3] = - ((right + left) / (right - left));
    }
    
    if (top != bottom)
    {
        aglCameraMat[ 4] = 0;
        aglCameraMat[ 5] = 2 / (top - bottom);
        aglCameraMat[ 6] = 0;
        aglCameraMat[ 7] = - ((top + bottom) / (top - bottom));
    }
	
    if (zFar != zNear)
    {
        aglCameraMat[ 8] = 0;
        aglCameraMat[ 9] = 0;
        aglCameraMat[10] = -2 / (zFar - zNear);
        aglCameraMat[11] = - ((zFar + zNear) / (zFar - zNear));
    }
	
	aglCameraMat[12] = 0;
	aglCameraMat[13] = 0;
	aglCameraMat[14] = 0;
	aglCameraMat[15] = 1;
}



GLvoid bindArrayBuffers()
{
}


GLvoid aglBindTexture()
{
	
}


GLvoid aglEnd() 
{
    /*
	GLboolean vertexArrayEnabled;
	GLboolean texCoordArrayEnabled;
	GLboolean colorArrayEnabled;
	
	GLvoid * vertexArrayPointer;
	GLvoid * texCoordArrayPointer;
	GLvoid * colorArrayPointer;
	
	GLint vertexArrayType, texCoordArrayType, colorArrayType;
	GLint vertexArraySize, texCoordArraySize, colorArraySize;
	GLsizei vertexArrayStride, texCoordArrayStride, colorArrayStride;
	
	bool resetPointers = false;
	
	glGetPointerv(GL_VERTEX_ARRAY_POINTER, &vertexArrayPointer);
	glGetPointerv(GL_TEXTURE_COORD_ARRAY_POINTER, &texCoordArrayPointer);
	glGetPointerv(GL_COLOR_ARRAY_POINTER, &colorArrayPointer);

	glGetBooleanv(GL_VERTEX_ARRAY, &vertexArrayEnabled);
	glGetBooleanv(GL_TEXTURE_COORD_ARRAY, &texCoordArrayEnabled);
	glGetBooleanv(GL_COLOR_ARRAY, &colorArrayEnabled);

	if (!vertexArrayEnabled)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
	}
	
	if (vertexArrayPointer != &aglesGlueArrays.vertices[0].xyz)
	{
		glGetIntegerv(GL_VERTEX_ARRAY_TYPE, &vertexArrayType);
		glGetIntegerv(GL_VERTEX_ARRAY_SIZE, &vertexArraySize);
		glGetIntegerv(GL_VERTEX_ARRAY_STRIDE, &vertexArrayStride);
		if (texCoordArrayEnabled)
		{
			glGetIntegerv(GL_TEXTURE_COORD_ARRAY_TYPE, &texCoordArrayType);
			glGetIntegerv(GL_TEXTURE_COORD_ARRAY_SIZE, &texCoordArraySize);
			glGetIntegerv(GL_TEXTURE_COORD_ARRAY_STRIDE, &texCoordArrayStride);
		}	
		if (colorArrayEnabled)
		{
			glGetIntegerv(GL_COLOR_ARRAY_TYPE, &colorArrayType);
			glGetIntegerv(GL_COLOR_ARRAY_SIZE, &colorArraySize);
			glGetIntegerv(GL_COLOR_ARRAY_STRIDE, &colorArrayStride);
		}
        

        
		//glVertexPointer(3, GL_FLOAT, sizeof(aglesVertex_t), aglesGlueArrays.vertices[0].xyz);
		//glTexCoordPointer(2, GL_FLOAT, sizeof(aglesVertex_t), aglesGlueArrays.vertices[0].st);
		//glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(aglesVertex_t), aglesGlueArrays.vertices[0].rgba);
		
		resetPointers = true;
	}
	
	if (!texCoordArrayEnabled)
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	if (!colorArrayEnabled)
		glEnableClientState(GL_COLOR_ARRAY);
	*/
    
    //glUseProgram(aglesShaderProgram);
    
    //glUniformMatrix4fv(aglCameraUniform, 1, GL_FALSE, aglCameraMat);
    
	if (aglesGlueArrays.currIndex == 0) 
	{
		aglesCurrentPrimitive = 0;
		return;
	}
    aglError("a");
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, 0, sizeof(aglesVertex_t), aglesGlueArrays.vertices[0].xyz);
    glVertexAttribPointer(ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, 0, sizeof(aglesVertex_t), aglesGlueArrays.vertices[0].rgba);
	
	if (aglesCurrentPrimitive == GL_QUADS) 
	{
		glDrawElements(GL_TRIANGLES, aglesGlueArrays.currIndex / 4 * 6, GL_UNSIGNED_SHORT, aglesGlueArrays.quadIndices);
	} 
	else 
	{
		glDrawArrays(aglesCurrentPrimitive, 0, aglesGlueArrays.currIndex);
	}
	aglesGlueArrays.currIndex = 0;
	aglesCurrentPrimitive = 0;
    
    
	
    /*
	if (resetPointers)
	{
		if (vertexArrayEnabled)
		{
			glVertexPointer(vertexArraySize, vertexArrayType, 
							vertexArrayStride, vertexArrayPointer);	
		}
		if (texCoordArrayEnabled)
		{
			glTexCoordPointer(texCoordArraySize, texCoordArrayType, 
							  texCoordArrayStride, texCoordArrayPointer);
		}
		if (colorArrayEnabled)
		{
			glColorPointer(colorArraySize, colorArrayType, 
						   colorArrayStride, colorArrayPointer);
		}
	}
	
	if (!vertexArrayEnabled)
		glDisableClientState(GL_VERTEX_ARRAY);
	
	if (!texCoordArrayEnabled)
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	if (!colorArrayEnabled)
		glDisableClientState(GL_COLOR_ARRAY);
     */
    aglError("z");
}


GLvoid aglError(const char *source)
{
	GLenum error = glGetError();
	 
	switch (error) {
		case GL_NO_ERROR:
			break;
		case GL_INVALID_ENUM:
			printf("GL Error (%x): GL_INVALID_ENUM. %s\n\n", error, source);
			break;
		case GL_INVALID_VALUE:
			printf("GL Error (%x): GL_INVALID_VALUE. %s\n\n", error, source);
			break;
		case GL_INVALID_OPERATION:
			printf("GL Error (%x): GL_INVALID_OPERATION. %s\n\n", error, source);
			break;
		case GL_OUT_OF_MEMORY:
			printf("GL Error (%x): GL_OUT_OF_MEMORY. %s\n\n", error, source);
			break;
		default:
			printf("GL Error (%x): %s\n\n", error, source);
			break;
	}
}

