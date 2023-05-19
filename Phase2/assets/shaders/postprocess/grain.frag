#version 330
uniform sampler2D tex;
uniform float time;
in vec2 tex_coord;
out vec4 frag_color;

void main() {
    // Define the wave parameters
    float frequency = 5.0;   // Controls the frequency of the waves
    float amplitude = 0.1;   // Controls the amplitude of the waves
    float speed = 1.0;       // Controls the speed of the waves

    // Calculate the displacement based on time and texture coordinates
    float displacement = amplitude * sin(frequency * tex_coord.x + time * speed);

    // Apply the displacement to the texture coordinates
    vec2 distorted_coord = vec2(tex_coord.x, tex_coord.y + displacement);

    // Sample the color from the original texture using the distorted coordinates
    vec4 color = texture(tex, distorted_coord);

    // Set the final color
    frag_color = color;
}
