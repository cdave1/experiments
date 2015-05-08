// Used for shadow lookup
varying vec4 ShadowCoord;



void main()
{
    // Shadow coord is rendered from the perspective of the light, BUT
    // these matrix values are pulled directly out of the open GL matrices.
    //
    // Also gl_vertex is the vertex rendered from the perspective of the camera.
    //
    // In our code, vertex is rendering a texture, so we actually need a position from the
    // geometry that was rendered.
    ShadowCoord = gl_TextureMatrix[7] * gl_Vertex;
    
    // Position is the vertex position multiplied by the projection and model view matrix.
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
    gl_FrontColor = gl_Color;
}

