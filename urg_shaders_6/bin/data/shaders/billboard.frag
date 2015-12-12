#version 150

// color of all points, passed in draw
uniform vec4 globalColor;

uniform sampler2D tex;

// output of every fragment shader (a pixel)
out vec4 outputColor;

// main fragment shader function
void main (void) {
    
    // set the output color to the global color
    outputColor = texture(tex, gl_PointCoord) * globalColor;
    
}