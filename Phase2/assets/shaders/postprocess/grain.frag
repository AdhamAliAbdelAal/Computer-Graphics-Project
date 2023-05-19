#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;

out vec4 frag_color;

// Grain parameters
float grain_amount = 0.1; // Adjust this value to control the intensity of the grain effect
float grain_size = 3;   // Adjust this value to control the size of the grain particles

void main() {
    // Get the original color of the pixel
    vec4 original_color = texture(tex, tex_coord);

    // Generate random noise based on texture coordinate
    float random_value = fract(sin(dot(tex_coord, vec2(12.9898, 78.233))) * 43758.5453);

    // Apply grain effect by adding noise to the original color
    vec4 grain_color = original_color + vec4(random_value * grain_amount);

    // Apply grain size by reducing color variation
    grain_color = floor(grain_color * grain_size) / grain_size;

    // Set the final color with grain effect
    frag_color = grain_color;
}