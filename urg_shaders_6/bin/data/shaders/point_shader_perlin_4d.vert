#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;

// time passed into the shader each draw
uniform float time;

// main program, called for each vertex
void main() {
    
    // make copy of position vector to be able to manipulate it
    vec4 newPosition = position;

    vec4 noiseOutput = noise4(vec4(position.x / 10000., position.y / 10000., position.z / 10000., time / 10000.));
//    float offset = (noiseOutput.x + noiseOutput.y + noiseOutput.z + noiseOutput.w) / 4.;
//    newPosition.y += offset;
    noiseOutput.w = 0;
    vec4 scaledOutput = normalize(noiseOutput);
    int exponent = 3;
    scaledOutput.x = pow(scaledOutput.x, exponent);
    scaledOutput.y = pow(scaledOutput.y, exponent);
    scaledOutput.z = pow(scaledOutput.z, exponent);
    newPosition += scaledOutput * 100;
    
    // call at end of modification of vertices to set new position
    // Note: Never change position directly (always deep copy it)
    gl_Position = modelViewProjectionMatrix * newPosition;
}

