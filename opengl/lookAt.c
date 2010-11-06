#include <stdio.h>
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <math.h>

/**
 * David Petrie 2010
 *
 * Very basic test of a camera defined by three vectors
 * using an implementation of the standard "lookAt" matrix
 * function.
 */

#define pi 3.141959f

typedef unsigned int uint32_t;

typedef struct vec3
{
	void Set(float _x, float _y, float _z) { _x = x; _y = y; _z = z; }
	float x; float y; float z;
} vec3_t;


typedef struct camera
{
	vec3_t eye;
	vec3_t center;
	vec3_t up;
}
camera_t;


typedef struct surface
{
	vec3_t polygon[4];
	vec3_t color;
} surface_t;


typedef struct matrix4
{
	float f[16];
} matrix4_t;


const uint32_t kMaxSurfaces = 16;
static surface_t g_surfaces[kMaxSurfaces];
static camera_t g_camera;
static uint32_t surfaceCount = 0;

vec3_t vec3Make(float x, float y, float z);
void test_lookAt(float eyex, float eyey, float eyez,
					float centerx, float centery, float centerz,
					float upx, float upy, float upz);

void PushCameraMatrix();
void Draw();
void PopCameraMatrix();

int main(int argc, char **argv)
{
	g_camera.eye.Set(0,0,0);
	g_camera.center.Set(10,0,0);
	g_camera.up.Set(0,1,0);
	
	
	// Make some walls
	{
		surface_t wall1, wall2, wall3, wall4, floor, roof;
		
		wall1.polygon[0] = vec3Make(4, -4, -10);
		wall1.polygon[1] = vec3Make(4, 4,  -10);
		wall1.polygon[2] = vec3Make(4, 4,  20);
		wall1.polygon[3] = vec3Make(4, -4, 20);
		g_surfaces[surfaceCount++] = wall1;
		
		wall2.polygon[0] = vec3Make(4, -4, 20);
		wall2.polygon[1] = vec3Make(4, 4,  20);
		wall2.polygon[2] = vec3Make(-16, 4,  20);
		wall2.polygon[3] = vec3Make(-16, -4, 20);
		g_surfaces[surfaceCount++] = wall2;
		
		wall3.polygon[0] = vec3Make(-16, -4, 20);
		wall3.polygon[1] = vec3Make(-16, 4,  20);
		wall3.polygon[2] = vec3Make(-16, 4,  -10);
		wall3.polygon[3] = vec3Make(-16, -4, -10);
		g_surfaces[surfaceCount++] = wall3;
		
		wall4.polygon[0] = vec3Make(-16, -4, -10);
		wall4.polygon[1] = vec3Make(-16, 4,  -10);
		wall4.polygon[2] = vec3Make(4, 4,  -10);
		wall4.polygon[3] = vec3Make(4, -4, -10);
		g_surfaces[surfaceCount++] = wall4;
		
		floor.polygon[0] = vec3Make(4, -4, -10);
		floor.polygon[1] = vec3Make(4, -4, 20);
		floor.polygon[2] = vec3Make(-16, -4, 20);
		floor.polygon[3] = vec3Make(-16, -4, -10);
		g_surfaces[surfaceCount++] = floor;
		
		roof.polygon[0] = vec3Make(-16, 4, -10);
		roof.polygon[1] = vec3Make(-16, 4, 20);
		roof.polygon[2] = vec3Make(4, 4, 20);
		roof.polygon[3] = vec3Make(4, 4, -10);
		g_surfaces[surfaceCount++] = roof;
	}
	
	
	while (1)
	{
		PushCameraMatrix();
		Draw();
		PopCameraMatrix();
	}
	
	return 1;
}


vec3_t vec3Make(float x, float y, float z)
{
	vec3_t vec;
	vec.x = x;
	vec.y = y;
	vec.z = z;
	return vec;
}


void PushCameraMatrix()
{
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glPushMatrix();
	
	//matrix4_t mProjection;
	
	//MatrixPerspectiveFovRH(mProjection, 90.0f*(pi/180.0f), 1.0f/1.5f, 1.0f, 10000.0f, false);
	//glMultMatrixf(mProjection.f);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPushMatrix();
	
	test_lookAt(
			  g_camera.eye.x, g_camera.eye.y, g_camera.eye.z,
			  g_camera.center.x, g_camera.center.y, g_camera.center.z,
			  g_camera.up.x,g_camera.up.y, g_camera.up.z
			  );
}


void Draw()
{
	glDisable(GL_TEXTURE_2D);
	
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glCullFace(GL_FRONT);
	
	surface_t surf;
	
	float f[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	for (unsigned i = 0; i < surfaceCount; ++i)
	{
		float color = (float)i/surfaceCount;
		surf = g_surfaces[i];
		glBegin(GL_QUADS);
		glColor4f(color, color, color, 1.0f);
		for (unsigned j = 0; j < 4; ++j)
		{
			glVertex3f(surf.polygon[j].x,
					   surf.polygon[j].y,
					   surf.polygon[j].z);
		}
		glEnd();
	}
	
	
	glBegin(GL_LINES);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(10.0f, 0.0f, 0.0f);
	glEnd();
	
	glBegin(GL_LINES);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 10.0f, 0.0f);
	glEnd();
	
	glBegin(GL_LINES);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 10.0f);
	glEnd();
	
	
	glDepthMask(GL_FALSE);
	glEnable(GL_DEPTH_TEST);
	
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}


void PopCameraMatrix()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}



void test_lookAt(float eyex, float eyey, float eyez,
				 float centerx, float centery, float centerz,
				 float upx, float upy, float upz)
{
    float m[16];
    float x[3], y[3], z[3];
    float mag;
	
    /* Make rotation matrix */
	
    /* Z vector */
    z[0] = eyex - centerx;
    z[1] = eyey - centery;
    z[2] = eyez - centerz;
    mag = (float)sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {			/* mpichler, 19950515 */
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }
	
    /* Y vector */
    y[0] = upx;
    y[1] = upy;
    y[2] = upz;
	
    /* X vector = Y cross Z */
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];
	
    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];
	
    /* mpichler, 19950515 */
    /* cross product gives area of parallelogram, which is < 1.0 for
     * non-perpendicular unit-length vectors; so normalize x, y here
     */
	
    mag = (float)sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }
	
    mag = (float)sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }
	
#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0;
    M(3, 0) = 0.0;
    M(3, 1) = 0.0;
    M(3, 2) = 0.0;
    M(3, 3) = 1.0;
#undef M
    {
        int a;
        float fixedM[16];
        for (a = 0; a < 16; ++a)
            fixedM[a] = (m[a]);
        glMultMatrixf(fixedM);
    }
	
    /* Translate Eye to Origin */
    glTranslatef((-eyex),
                 (-eyey),
                 (-eyez));
}


