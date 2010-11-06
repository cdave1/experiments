//
//  Particle.mm
//  ParticleFun
//
//  Created by david on 26/02/09.
//  Copyright 2009 n/a. All rights reserved.
//

#import "ParticleEmitter.h"


@implementation ParticleEmitter

- init
{
	if ((self = [super init]))
	{
		memset(particles, 0, sizeof(particles));
		memset(particlePoints, 0, sizeof(particlePoints));
		memset(particleColors, 0, sizeof(particleColors));
		
		//world = new hdGameWorld(vec3_t(10.0, 10.0, 10.0), vec3_t(3.2, 4.8, 2.0));
		[ParticleEmitter LoadPNGTexture:@"star.png" handle:&particleTexture];
		[ParticleEmitter LoadPNGTexture:@"particle_mask.png" handle:&maskTexture];
		
		glEnable(GL_TEXTURE_2D);
		
		glEnable(GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		explosionDuration = 0.0;
	}
	return self;
}


+ (void)LoadPNGTexture:(NSString *)fileName handle:(GLuint *)nameHandle
{
	
	CGImageRef spriteImage;
	size_t	width, height;
	CGContextRef spriteContext;
	GLubyte *spriteData;
	
	
	// Creates a Core Graphics image from an image file
	spriteImage = [UIImage imageNamed:fileName].CGImage;
	// Get the width and height of the image
	width = CGImageGetWidth(spriteImage);
	height = CGImageGetHeight(spriteImage);
	// Texture dimensions must be a power of 2. If you write an application that allows users to supply an image,
	// you'll want to add code that checks the dimensions and takes appropriate action if they are not a power of 2.
	
	
	
	
	if(spriteImage) {
		// Allocated memory needed for the bitmap context
		spriteData = (GLubyte *) malloc(width * height * 4);
		// Uses the bitmatp creation function provided by the Core Graphics framework. 
		spriteContext = CGBitmapContextCreate(spriteData, width, height, 8, width * 4, CGImageGetColorSpace(spriteImage), kCGImageAlphaPremultipliedLast);
		// After you create the context, you can draw the sprite image to the context.
		CGContextDrawImage(spriteContext, CGRectMake(0.0, 0.0, (CGFloat)width, (CGFloat)height), spriteImage);
		// You don't need the context at this point, so you need to release it to avoid memory leaks.
		CGContextRelease(spriteContext);
		
		// Use OpenGL ES to generate a name for the texture.
		//glGenTextures(1, &spriteTexture);
		glGenTextures(1, nameHandle);
		// Bind the texture name. 
		//glBindTexture(GL_TEXTURE_2D, spriteTexture);
		glBindTexture(GL_TEXTURE_2D, *nameHandle);		
		
		// Speidfy a 2D texture image, provideing the a pointer to the image data in memory
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, spriteData);
		// Release the image data
		
		glGenerateMipmapOES(GL_TEXTURE_2D);
		
		free(spriteData);		
	}
}


- (void)Explode:(float)x y:(float)y;
{
	explodeX = float(x) / float(100);
	explodeY = float(4.8) - (float(y) / float(100));
	explosionDuration = EXPLOSION_DURATION;
}



- (void)DrawParticles:(NSTimeInterval)elapsed
{
	int MaxSpread,MaxParticles;
	float Spread,Angle; 
	
	MaxSpread=INITIAL_PARTICLE_SPREAD;
	MaxParticles=NUM_PARTICLES/2;
	
	ticks = elapsed * 2000; //WHY????
	
	if (explosionDuration <= 0.0) return;
	
	explosionDuration -= elapsed;
	
	for (int i = 0; i < MaxParticles; i++)
	{
		if (particles[i].life > 0.0)
		{
			float xPos = (particles[i].position.x + particles[i].direction.x * ticks);
			float yPos = (particles[i].position.y + particles[i].direction.y * ticks);
			float zPos = (particles[i].position.z + particles[i].direction.z * ticks);
			
			// falling
			//particles[i].direction.y = -((1.0 - particles[i].life) * SPEED_DECAY * ticks);
			
			
			particles[i].position = vec3_t(xPos, yPos, zPos);
			particles[i].life -= (0.001f * ticks);
		}
		else
		{
			if (explosionDuration > (EXPLOSION_DURATION / 2.0))
			{
				particles[i].position = vec3_t(0, 0, 0);
				particles[i].life = EXPLOSION_DURATION;
				
				Spread=(float)(rand()%MaxSpread)/10000.0f;
				Angle=(float)(rand()%360); ///100.0f; // Quarter circle
				
				// Calculate X and Z vectors
				particles[i].direction.x = cos(Angle)*Spread;
				particles[i].direction.z = 0.0; //sin(Angle)*Spread;
				particles[i].direction.y = sin(Angle)*Spread;
			}
		}
		
		particlePoints[i * 3] = particles[i].position.x;
		particlePoints[(i * 3) + 1] = particles[i].position.y;
		particlePoints[(i * 3) + 2] = particles[i].position.z;
		
		particleColors[i * 4] = 255;
		particleColors[(i * 4) + 1] = 255;
		particleColors[(i * 4) + 2] = 255;
		particleColors[(i * 4) + 3] = (GLubyte)MAX(0, (particles[i].life * 255));
		
	}
	
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glVertexPointer(3, GL_FIXED, 0, particlePoints);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, particleColors);
	
		
	
	
	GLfloat sizes[2];
	glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, sizes);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_POINT_SPRITE_OES);
	float quadratic[] =  { 1.0f, 0.0f, 0.01f };
	glPointParameterfv( GL_POINT_DISTANCE_ATTENUATION, quadratic );
	glPointParameterf( GL_POINT_FADE_THRESHOLD_SIZE, 32.0f );
	glPointParameterf( GL_POINT_SIZE_MIN, sizes[0] );
	glPointParameterf( GL_POINT_SIZE_MAX, sizes[1] );
	glTexEnvi(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_TRUE);
	glPointSize(16.0f);
	glBindTexture(GL_TEXTURE_2D, particleTexture);
	glDepthMask(GL_FALSE);
	
	
	glPushMatrix();
	glTranslatex(explodeX,
				 explodeY,
				 0);
	glDrawArrays(GL_POINTS, 0, MaxParticles);
	
	glPopMatrix();
	glDisableClientState(GL_VERTEX_ARRAY); 
	glDisableClientState(GL_COLOR_ARRAY); 
	
	glEnable(GL_DEPTH_TEST); 
	glDisable(GL_BLEND); 
	glTexEnvx(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_FALSE); 
	glDisable(GL_TEXTURE_2D); 
	glDisable(GL_POINT_SPRITE_OES); 
}

@end
