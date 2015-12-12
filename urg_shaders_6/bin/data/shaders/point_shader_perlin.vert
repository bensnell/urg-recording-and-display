#version 150

uniform mat4 modelViewProjectionMatrix;
in vec4 position;

// time passed into the shader each draw
uniform float time;

// -------------------------------------------------------------------

// perlin noise generator from Kyle McDonald and Tim S
// https://forum.openframeworks.cc/t/ofxshader-mesh-displacement-perlin-noise/4256/10

const int octaves = 5;
const vec3 rDot = vec3(12.9898, 78.233, 42.43);
const float rScale = 43758.5453;

//uniform int octaves;
uniform float seed;
uniform float[6] weights;
uniform float[6] scaling;
uniform float normalization;


float pi = (asin(1.) * 2.0);
vec3 fractPos,floorPos;
vec4 bot,top,zMix;
vec2 yMix;

float rand(vec3 coord) {
    return fract(sin(dot(coord.xyz, rDot)) * rScale);
}

float bilinearRand(vec3 pos) {
    fractPos = (1. - cos(fract(pos)*pi))*0.5;
    floorPos = floor(pos);
    
    bot = vec4(
               rand(floorPos),
               rand(floorPos + vec3(1,0,0)),
               rand(floorPos + vec3(0,1,0)),
               rand(floorPos + vec3(1,1,0)));
    
    top = vec4(
               rand(floorPos + vec3(0,0,1)),
               rand(floorPos + vec3(1,0,1)),
               rand(floorPos + vec3(0,1,1)),
               rand(floorPos + vec3(1,1,1)));
    
    zMix = mix(bot,top, fractPos.z);
    yMix = mix(zMix.xy, zMix.zw, fractPos.y);
    return mix(yMix.x,yMix.y, fractPos.x);
}

float noise(vec3 pos) {
    float total = 0.;
    for(int i = 0; i < octaves; i++)
        total += weights[i] * bilinearRand((pos + seed * float(i + 1)) * scaling[i]);
    return total * normalization;
}

// -------------------------------------------------------------------

// main program, called for each vertex
void main() {
    
    // make copy of position vector to be able to manipulate it
    vec4 newPosition = position;

    // perlin noise offset
    float offset = noise(vec3(position.x / 1000., position.y / 1000., time / 1000. + position.x / 1000.));
    newPosition.y += offset;
    
    // call at end of modification of vertices to set new position
    // Note: Never change position directly (always deep copy it)
    gl_Position = modelViewProjectionMatrix * newPosition;
}

