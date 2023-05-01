#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

// How far (in the texture space) is the distance (on the x-axis) between
// the pixels from which the red/green (or green/blue) channels are sampled
#define STRENGTH 0.005

// Chromatic aberration mimics some old cameras where the lens disperses light
// differently based on its wavelength. In this shader, we will implement a
// cheap version of that effect 

void main(){
    //TODO: Modify this shader to apply chromatic abberation
    // To apply this effect, we only read the green channel from the correct pixel (as defined by tex_coord)
    vec4  g = texture(tex, tex_coord);
    // To get the red channel, we move by amount STRENGTH to the left then sample another pixel from which we take the red channel
    vec4  r = texture(tex, vec2(tex_coord.x - STRENGTH,tex_coord.y));
    // To get the blue channel, we move by amount STRENGTH to the right then sample another pixel from which we take the blue channel
    vec4  b = texture(tex, vec2(tex_coord.x + STRENGTH,tex_coord.y));
    
    // red channel : r.x
    // green channel : g.y
    // blue channel : b.z
    frag_color = vec4(r.x, g.y, b.z, 1.0);
}