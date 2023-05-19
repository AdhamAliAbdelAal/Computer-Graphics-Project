#version 330
uniform sampler2D tex;
uniform float time;
uniform float speed=5;
uniform float amplitude=0.05;
uniform float frequency=3;
in vec2 tex_coord;
out vec4 frag_color;

void main() {
    // Calculate the displacement based on time and texture coordinates
    float displacement = amplitude * sin(frequency * tex_coord.y + time * speed);

    // Apply the displacement to the horizontal position
    float x = tex_coord.x + displacement;

    // Sample the color from the original texture using the modified coordinates
    vec4 color = texture(tex, vec2(x, tex_coord.y));

    // Set the final color
    frag_color = color;
}
