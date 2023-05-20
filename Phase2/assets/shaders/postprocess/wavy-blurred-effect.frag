#version 330
uniform sampler2D tex;
uniform float time;  // Uniform variable for time
uniform float speed=5;
uniform float amplitude=0.05;
uniform float frequency=3;
in vec2 tex_coord;
out vec4 frag_color;

// The number of samples we read to compute the blurring effect
#define STEPS 16
// The strength of the blurring effect
#define STRENGTH 0.2

void main() {

    // To apply radial blur, we compute the direction outward from the center to the current pixel
    vec2 step_vector = (tex_coord - 0.5) * (STRENGTH / STEPS);

    // Then we sample multiple pixels along that direction and compute the average
    for(int i = 0; i < STEPS; i++){
        // Calculate the displacement based on time and texture coordinates
        float displacement = amplitude * sin(frequency * tex_coord.y + time * speed);

        // Apply the displacement to the horizontal position
        float x = tex_coord.x + displacement;

        // Sample the color from the original texture using the modified coordinates
        frag_color += texture(tex, vec2(x, tex_coord.y));
        
        frag_color += texture(tex, tex_coord + step_vector * i);    
    }

    frag_color /= STEPS;


}
