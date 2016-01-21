// 2D Distance fields
//
// 1. Configurable distance from a 2D polygon, where a polygon is a collection of edges
// 

const int MAX_COORDS = 4;
vec2 polygon[MAX_COORDS];


// https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
float lineDistance(in vec2 uv, in vec2 p1, in vec2 p2) {
    float n = abs((p2.y - p1.y)*uv.x - (p2.x - p1.x)*uv.y + (p2.x * p1.y) + (p2.y * p1.x));
    float d = sqrt(pow(p2.y - p1.y, 2.) + pow(p2.x - p1.x, 2.));
    return abs(n/d) * 10.;
}


float segmentDistance(in vec2 uv, in vec2 p1, in vec2 p2) {
    vec2 line = p2 - p1;
    // u =  ((x3 - x1) * px + (y3 - y1) * py) / float(something)
    float u = ((uv.x - p1.x) * line.x + (uv.y - p1.y) * line.y) / (line.x * line.x + line.y * line.y);
	u = clamp(u, 0., 1.);
    
    float x = (p1.x + u * line.x) - uv.x;
    float y = (p1.y + u * line.y) - uv.y;
    
    return sqrt(x * x + y * y) * 10.;
}


float polygonDistance(in vec2 uv) {
    float winner = 1.;
    for (int i = 0; i < MAX_COORDS; ++i) {
        float distance;
        
        vec2 a = polygon[i];
        vec2 b = polygon[int(mod(float(i + 1), float(MAX_COORDS)))];
        
        if (false) { //fract(iGlobalTime * 0.5) < 0.5) {
			distance = lineDistance(uv, a, b);
    	} else {
			distance = segmentDistance(uv, a, b);
    	}
        
        if (distance < winner) {
            winner = distance;
        }
    }
    return winner;
}


void mainImage( out vec4 fragColor, in vec2 fragCoord ) {
	vec2 uv = fragCoord.xy / iResolution.xy;

    polygon[0] = vec2(0.25, 0.25);
    polygon[1] = vec2(0.75, 0.75);
    polygon[2] = vec2(0.25, 0.75);
    
	 float distance = polygonDistance(uv);
    
    fragColor = vec4(vec3(fract(1.0 - distance)), 1.);
}
