#version 330
uniform sampler2D tex;
uniform float time;
in vec2 tex_coord;
out vec4 frag_color;


void main() {

    // Battery charge color
    vec3 battery_color = vec3(0.0, 1.0, 0.0);

    // Get the original scene color
    vec4 original_color = texture(tex, tex_coord);
    
    // Calculate the blend factor based on the battery level
    float blend_factor = clamp(1.0 - (0.5 + tex_coord.y), 0.0, 0.5);
    
    // Create a color gradient based on the battery level
    vec3 blended_color = mix(original_color.rgb, battery_color, blend_factor);
    
    // Apply the color to the final output
    frag_color = vec4(blended_color, original_color.a);
}
