#version 330 core

out vec4 frag_color;

// In this shader, we want to draw a checkboard where the size of each tile is (size x size).
// The color of the bottom-left most tile should be "colors[0]" and the 2 tiles adjacent to it
// should have the color "colors[1]".

//TODO: (Req 1) Finish this shader.

uniform int size = 32;
uniform vec3 colors[2];

void main(){
    // gl_FragCoord.x and gl_FragCoord.y are the x and y coordinates respectively.
    // gl_FragCoord.z and gl_FragCoord.w are the z and w coordinates respectively.
    // gl_FragCoord is in screen coordinates.

    // STEP(1): we divide each coordinate by the size of tile to know the index of the tile the pixel belongs to in x and y direction.
    vec4 Position = floor(gl_FragCoord/size);
    // STEP(2): we add the x and y index to know if the tile is even or odd.
    float total = mod (mod(Position.x, 2.0) + mod(Position.y, 2.0),2.0);
    // STEP(3): we use the total to know which color to use.
    // if the total is 0, we use the first color, otherwise we use the second color. 
    frag_color = vec4(colors[int(total)],1.0);
}