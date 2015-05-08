// We want to translate a screen position into a box rendering.
//
// To do that, we need to cast a ray from each fragment location (uv).
// We "project" the ray into a function that returns a pixel color.
//
// We're saying "at this position on the screen, the scene looks like this" and
// we return the fragment at that position.
//
// Movement can be simple -- we apply some offset to the ray position and convert into
// a vec3 ray.  As long as our rendering function takes a vec3 as the ray, then we should be good!
//
// Two strategies:
// - Use traditional ray tracing where we cast a ray and see if it hits geometry.
// - Use ray marching, same concept except we step each ray
 
const float pi = 3.14159265359;
 
vec3 offy(float i) {
    float f = sin(iGlobalTime) * .5 + .5;
    return vec3(0);//texture2D(iChannel0, vec2(i * 0.07 + f, i * 0.09 +f)).xy, 0.);
}
 
struct Ray {
    vec3 origin;
    vec3 direction;
};
   
struct AABB {
    vec3 lower;
    vec3 upper;
};
 
 
mat3 rotateX(float th) {
    float c = cos(th);
    float s = sin(th);
    return mat3(1, 0, 0,
                0, c, -s,
                0, s, c);
}
 
 
mat3 rotateY(float th) {
    float c = cos(th);
    float s = sin(th);
    return mat3(c, 0, s,
                0, 1, 0,
                -s,0, c);
}
 
// via http://prideout.net/blog/?p=64
bool IntersectBox(Ray r, AABB box, out float t0, out float t1)
{
    vec3 invR = 1.0 / r.direction;
    vec3 tbot = invR * (box.lower - r.origin);
    vec3 ttop = invR * (box.upper - r.origin);
    vec3 tmin = min(ttop, tbot);
    vec3 tmax = max(ttop, tbot);
    vec2 t = max(tmin.xx, tmin.yz);
    t0 = max(t.x, t.y);
    t = min(tmax.xx, tmax.yz);
    t1 = min(t.x, t.y);
    return t0 <= t1;
}
 
 
bool pointInsideBox(vec3 point, AABB box) {    
    vec3 lower = box.lower;
    vec3 upper = box.upper;
 
    return (lower.x <= point.x && upper.x >= point.x &&
            lower.y <= point.y && upper.y >= point.y &&
            lower.z <= point.z && upper.z >= point.z);
}
 
 
float rayDistance;
bool shittyRayMarch(Ray ray, AABB box) {
    const float steps = 40.;
    const float fraction = 0.5 / steps;
   
    vec3 delta = vec3(0.);
    ///for (float i = 0.; i < steps; i += 1.) {
        //delta = delta + (fraction * ray.direction);
        //if (pointInsideBox(ray.origin + delta, box)) {
            float t0, t1;
            if (IntersectBox(ray, box, t0, t1)) {
                rayDistance = (1. - t0); //distance(ray, delta));
                return true;
            }
        //}
    //}
    return false;
}
 
 
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
    vec2 uv = fragCoord.xy / iResolution.xy;
   
    float st = 0.15 * pi * sin(iGlobalTime);
   
    mat3 rot = rotateY(st) * rotateX(st);
    vec3 origin = rot * vec3(uv, 0.0);
    vec2 uvp = (uv * 2. - 1.) * .4;
    vec3 direction = normalize(rot * vec3(uvp.x, uvp.y, 1.0));
   
    Ray ray = Ray (origin, direction);
   
    AABB box1 = AABB( vec3(0.25, 0.0, 0.15), vec3(0.55, 0.55, 2.55));
    AABB box2 = AABB( vec3(0.65, 0.65, 0.55), vec3(0.85, 0.85, 0.85));
    AABB box3 = AABB( vec3(0.0, 0.65, 0.95), vec3(0.85, 0.85, 0.85));
   
    if (shittyRayMarch(ray, box1)) {
        fragColor = vec4(rayDistance, rayDistance, rayDistance, 1.);
        return;
    } else if (shittyRayMarch(ray, box2)) {
        fragColor = vec4(rayDistance, rayDistance, rayDistance, 1.);
        return;
    } else if (shittyRayMarch(ray, box3)) {
        fragColor = vec4(rayDistance, rayDistance, rayDistance, 1.);
        return;
    }
    fragColor = vec4(0);
    // uv,0.5+0.5*sin(iGlobalTime)
   
}