//
//  Particle.h
//  ParticleFun
//
//  Created by david on 26/02/09.
//  Copyright 2009 n/a. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "Texture2D.h"

typedef struct vec3_t
{
	vec3_t () {}	
	vec3_t (float x, float y, float z) : x(x), y(y), z(z) {}
	float x,y,z;
};

struct Particle
{
	vec3_t position;
	vec3_t direction;
	NSTimeInterval life;
};

#define EXPLOSION_DURATION 0.6
#define PARTICLE_SIZE 0.5f
#define NUM_PARTICLES 256
#define INITIAL_PARTICLE_SPREAD 10
#define SPEED_DECAY 0.0001f // (Gravity)
//const NSTimeInterval kMaxParticleLifetime = 2.0;
//const int kParticleCount = 512;

@interface ParticleEmitter : NSObject {
	//hdGameWorld* world;	
	struct Particle particles[NUM_PARTICLES];
	float particlePoints[NUM_PARTICLES * 3];
	GLubyte particleColors[NUM_PARTICLES * 4];
	//FIXED32 particleTexture
	
	int particleCount;
	int ticks;
	GLuint maskTexture;
	GLuint particleTexture;
	
	CFTimeInterval explosionDuration;
	float explodeX;
	float explodeY;
}
+ (void)LoadPNGTexture:(NSString *)fileName handle:(GLuint *)nameHandle;
- (void)DrawParticles:(NSTimeInterval)elapsed;
- (void)Explode:(float)x y:(float)y;
@end
