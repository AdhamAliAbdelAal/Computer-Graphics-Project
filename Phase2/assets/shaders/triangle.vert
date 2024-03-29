#version 330

// This vertex shader should be used to render a triangle whose normalized device coordinates are:
// (-0.5, -0.5, 0.0), ( 0.5, -0.5, 0.0), ( 0.0,  0.5, 0.0)
// And it also should send the vertex color as a varying to the fragment shader where the colors are (in order):
// (1.0, 0.0, 0.0), (0.0, 1.0, 0.0), (0.0, 0.0, 1.0)

out Varying {
    vec3 color;
} vs_out;

uniform vec2 scale=vec2(1.0,1.0);
uniform vec2 translation=vec2(0.0,0.0);


// Currently, the triangle is always in the same position, but we don't want that.
// So two uniforms should be added: translation (vec2) and scale (vec2).
// Each vertex "v" should be transformed to be "scale * v + translation".
// The default value for "translation" is (0.0, 0.0) and for "scale" is (1.0, 1.0).

//TODO: (Req 1) Finish this shader

void main() {
    
    // Initialize the position and color of the vertices
    vec3 positions[3] = vec3[3](
        vec3(-0.5, -0.5, 0.0), 
        vec3(0.5, -0.5, 0.0), 
        vec3(0.0, 0.5, 0.0));

    vec3 colors[3] = vec3[3](
        vec3(1.0, 0.0, 0.0), 
        vec3(0.0, 1.0, 0.0), 
        vec3(0.0, 0.0, 1.0)
        );

    // STEP 1: append 1 to scale vector to make it a vec3 (the default value for scale is 1) 
    // STEP 2: append 0 to translation vector to make it a vec3 (the default value for translation is 0)
    // STEP 3: multiply the scale vector by the position vector and add the translation vector

    gl_Position = vec4(vec3(scale,1.0)*positions[gl_VertexID]+vec3(translation,0.0), 1.0);

    // Send the color to the fragment shader
    vs_out.color = colors[gl_VertexID];
}