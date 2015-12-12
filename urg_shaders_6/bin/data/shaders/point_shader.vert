#version 150
// ^ Is this needed?

// Automatically passed into vertex shader:

// combined model, view, and projection matrix of the app for each draw call
uniform mat4 modelViewProjectionMatrix;

// position of a vertex
in vec4 position;

//-------------------

// Manually passed into vertex shader:

// time passed into the shader each draw
uniform float time;

//-------------------

// main program, called for each vertex
void main() {
    
    // make copy of position vector to be able to manipulate it
    vec4 newPosition = position;

    // sine offset
    float offset = sin(time / 1000. + position.x / 1000.) * 50;
    newPosition.y += offset;
    
    // call at end of modification of vertices to set new position
    // Note: Never change position directly (always deep copy it)
    gl_Position = modelViewProjectionMatrix * newPosition;
}

