//
//  ES2Renderer.m
//  OpenGLTest3
//
//  Created by David Petrie on 18/05/10.
//  Copyright n/a 2010. All rights reserved.
//

#import "ES2Renderer.h"

// uniform index
typedef enum uniformIndex {
	ROT_MATRIX = 0,
	CAMERA_MATRIX,
	TRANSLATION_UNIFORM,
    NUM_UNIFORMS
} uniformIndex_t;


typedef struct camerast
{
	vec3_t eye;
	vec3_t center;
	vec3_t up;
} camera_t;

GLint uniformIndices[NUM_UNIFORMS];

// attribute index
static camera_t camera;
static float zRotate;
static float cameraMatrix[16]; // column major order
static float rotationMatrix[16];
vec4_t translationVector;

@interface ES2Renderer (PrivateMethods)
- (BOOL)loadShaders;
- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type file:(NSString *)file;
- (BOOL)linkProgram:(GLuint)prog;
- (BOOL)validateProgram:(GLuint)prog;
@end

@implementation ES2Renderer

// Create an OpenGL ES 2.0 context
- (id)init
{
    if ((self = [super init]))
    {
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
		
        if (!context || ![EAGLContext setCurrentContext:context] || ![self loadShaders])
        {
            [self release];
            return nil;
        }
		
        // Create default framebuffer object. The backing will be allocated for the current layer in -resizeFromLayer
        glGenFramebuffers(1, &defaultFramebuffer);
        glGenRenderbuffers(1, &colorRenderbuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
    }
	 

	
    return self;
}
static float zMove = 0.0f;
- (void)render
{
    // This application only creates a single context which is already set current at this point.
    // This call is redundant, but needed if dealing with multiple contexts.
    [EAGLContext setCurrentContext:context];
	
    // This application only creates a single default framebuffer which is already bound at this point.
    // This call is redundant, but needed if dealing with multiple framebuffers.
    glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
    glViewport(0, 0, backingWidth, backingHeight);
	
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
	
    // Use shader program
	glUseProgram(program);
	zMove += 0.05f;
	zRotate += 0.05f;
		//aglMatrixPerspectiveFovRH(cameraMatrix, 90.0f*(3.14159/180.0f), 0.75f, 1.0f, 10000.0f);
	
	// vec3Set Stuff
	vec3Set(camera.eye, 0.0f, 0.0f, -5.0f + sin(zMove));
	vec3Set(camera.center, sin(zMove), 0.0f, 10.0f);
	vec3Set(camera.up, 0.0f, 1.0f, 0.0f);
	aglMatrixLookAtRH(cameraMatrix, camera.eye, camera.center, camera.up);
	
	
	aglMatrixRotationZ(rotationMatrix, zRotate);
	
	vec4Set(translationVector, 0.0f, 0.0f, 10.0f, 0.0f);
	
	glUniformMatrix4fv(uniformIndices[ROT_MATRIX], 1, GL_FALSE, rotationMatrix);
	glUniformMatrix4fv(uniformIndices[CAMERA_MATRIX], 1, GL_FALSE, cameraMatrix);
	glUniform4fv(uniformIndices[TRANSLATION_UNIFORM], 1, translationVector);
	
	
	
	
	aglBegin(GL_TRIANGLE_STRIP);
	aglVertex3f(-0.5f, -0.5f, 0.0f);
	aglColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	aglVertex3f(0.5f, -0.5f, 0.0f);
	aglColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	aglVertex3f(-0.5f, 0.5f, 0.0f);
	aglColor4f(0.0f, 1.0f, 1.0f, 1.0f);
	aglVertex3f(0.5f, 0.5f, 0.0f);
	aglColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	aglEnd();
	
	
    // This application only creates a single color renderbuffer which is already bound at this point.
    // This call is redundant, but needed if dealing with multiple renderbuffers.
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [context presentRenderbuffer:GL_RENDERBUFFER];
}

- (BOOL)compileShader:(GLuint *)shader type:(GLenum)type file:(NSString *)file
{
    GLint status;
    const GLchar *source;
	
    source = (GLchar *)[[NSString stringWithContentsOfFile:file encoding:NSUTF8StringEncoding error:nil] UTF8String];
    if (!source)
    {
        NSLog(@"Failed to load vertex shader");
        return FALSE;
    }
	
    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, NULL);
    glCompileShader(*shader);
	
#if defined(DEBUG)
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        NSLog(@"Shader compile log:\n%s", log);
        free(log);
    }
#endif
	
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
        glDeleteShader(*shader);
        return FALSE;
    }
	
    return TRUE;
}

- (BOOL)linkProgram:(GLuint)prog
{
    GLint status;
	
    glLinkProgram(prog);
	
#if defined(DEBUG)
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        NSLog(@"Program link log:\n%s", log);
        free(log);
    }
#endif
	
    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0)
        return FALSE;
	
    return TRUE;
}

- (BOOL)validateProgram:(GLuint)prog
{
    GLint logLength, status;
	
    glValidateProgram(prog);
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        NSLog(@"Program validate log:\n%s", log);
        free(log);
    }
	
    glGetProgramiv(prog, GL_VALIDATE_STATUS, &status);
    if (status == 0)
        return FALSE;
	
    return TRUE;
}

- (BOOL)loadShaders
{
    GLuint vertShader, fragShader;
    NSString *vertShaderPathname, *fragShaderPathname;
	
    // Create shader program
    program = glCreateProgram();
	
    // Create and compile vertex shader
    vertShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"vsh"];
    if (![self compileShader:&vertShader type:GL_VERTEX_SHADER file:vertShaderPathname])
    {
        NSLog(@"Failed to compile vertex shader");
        return FALSE;
    }
	
    // Create and compile fragment shader
    fragShaderPathname = [[NSBundle mainBundle] pathForResource:@"Shader" ofType:@"fsh"];
    if (![self compileShader:&fragShader type:GL_FRAGMENT_SHADER file:fragShaderPathname])
    {
        NSLog(@"Failed to compile fragment shader");
        return FALSE;
    }
	
    // Attach vertex shader to program
    glAttachShader(program, vertShader);
	
    // Attach fragment shader to program
    glAttachShader(program, fragShader);
	
    // Bind attribute locations
    // this needs to be done prior to linking
	/*
	 
	 We bind the program to an index with a particular attribute name. The
	 index can then map to a specific buffer (eg, vertex), which will be 
	 accessible in the program using the given attribute name.
	 
	 What does this mean? It means there can potentially be multiple
	 shader programs applied to a single buffer, and thus interesting effects are relatively
	 straightforward. 
	 
	 In other words, if I bind a different program to the same index, that program
	 can apply its effects to the buffer. 
	 
	 Note: glLinkProgram is the command to call to make the attribute binding effective.
	 After this is called, the bindings are "fixed". I don't fully understand this.
	 
	 
	 */
    glBindAttribLocation(program, ATTRIB_VERTEX, "position");
    glBindAttribLocation(program, ATTRIB_COLOR, "color");
	
    // Link program
    if (![self linkProgram:program])
    {
        NSLog(@"Failed to link program: %d", program);
		
        if (vertShader)
        {
            glDeleteShader(vertShader);
            vertShader = 0;
        }
        if (fragShader)
        {
            glDeleteShader(fragShader);
            fragShader = 0;
        }
        if (program)
        {
            glDeleteProgram(program);
            program = 0;
        }
        
        return FALSE;
    }
	
	// Get uniform locations
	uniformIndices[ROT_MATRIX] = glGetUniformLocation(program, "rotation");
	uniformIndices[CAMERA_MATRIX] = glGetUniformLocation(program, "camera");
	uniformIndices[TRANSLATION_UNIFORM] = glGetUniformLocation(program, "translation");
	
    // Release vertex and fragment shaders
    if (vertShader)
        glDeleteShader(vertShader);
    if (fragShader)
        glDeleteShader(fragShader);
	
    return TRUE;
}

- (BOOL)resizeFromLayer:(CAEAGLLayer *)layer
{
    // Allocate color buffer backing based on the current layer size
    glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
    [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:layer];
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &backingWidth);
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &backingHeight);
	
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
        return NO;
    }
	
    return YES;
}

- (void)dealloc
{
    // Tear down GL
    if (defaultFramebuffer)
    {
        glDeleteFramebuffers(1, &defaultFramebuffer);
        defaultFramebuffer = 0;
    }
	
    if (colorRenderbuffer)
    {
        glDeleteRenderbuffers(1, &colorRenderbuffer);
        colorRenderbuffer = 0;
    }
	
    if (program)
    {
        glDeleteProgram(program);
        program = 0;
    }
	
    // Tear down context
    if ([EAGLContext currentContext] == context)
        [EAGLContext setCurrentContext:nil];
	
    [context release];
    context = nil;
	
    [super dealloc];
}

@end
