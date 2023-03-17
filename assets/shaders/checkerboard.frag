#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the bottom-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main(){

    vec4 Position = floor(gl_FragCoord/size);
    float total = mod (mod(Position.x, 2.0) + mod(Position.y, 2.0),2.0);
    frag_color = vec4(colors[int(total)],1.0);
}