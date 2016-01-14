#version 150
// ^ Is this needed?

// UNIFORMS
// can change from draw to draw but not within a shader call

// color of all points, passed in draw
uniform vec4 globalColor;

// output of every fragment shader (a pixel)
out vec4 outputColor;

// main fragment shader function
void main (void) {
    
    // set the output color to the global color
    outputColor = globalColor;
    
//	gl_FragColor
}
