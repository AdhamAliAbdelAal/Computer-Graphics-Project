#version 330
uniform sampler2D tex;
uniform float time;
in vec2 tex_coord;
out vec4 frag_color;

// Battery charge level (between 0.0 and 1.0)
float startValue = 0.4;
float endValue = 0.9;

// Battery charge color
vec3 battery_color = vec3(0.0, 1.0, 0.0);

void main() {

    // Interpolation factor
    float t = clamp(sin(time), startValue, endValue); // Clamp time

    // Interpolation Value
    float battery_level = t;
    
    // Get the original scene color
    vec4 original_color = texture(tex, tex_coord);
    
    // Calculate the blend factor based on the battery level
    float blend_factor = 1.0 - (0.5 + tex_coord.y);
    
    // Create a color gradient based on the battery level
    vec3 blended_color = mix(original_color.rgb, battery_color, blend_factor);
    
    // Apply the color to the final output
    frag_color = vec4(blended_color, original_color.a);
}
