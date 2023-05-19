#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;

out vec4 frag_color;

// Battery charge level (between 0.0 and 1.0)
float battery_level = 0.8;

// Battery charge color
vec3 battery_color = vec3(0.0, 1.0, 0.0);

void main() {
    // Get the original scene color
    vec4 original_color = texture(tex, tex_coord);
    
    // Calculate the blend factor based on the battery level
    float blend_factor = 1.0 - battery_level;
    
    // Create a color gradient based on the battery level
    vec3 blended_color = mix(original_color.rgb, battery_color, blend_factor);
    
    // Apply the color to the final output
    frag_color = vec4(blended_color, original_color.a);
}