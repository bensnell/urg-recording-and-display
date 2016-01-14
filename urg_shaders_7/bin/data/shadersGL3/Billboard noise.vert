#version 150

uniform mat4 orientationMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 textureMatrix;
uniform mat4 modelViewProjectionMatrix;

uniform float minPointDist;
uniform float maxPointDist;

uniform float pointSizeMultiplier;
uniform float pointSizePower;
uniform float minPointSize;
uniform float maxPointSize;

uniform float pointOpacityMultiplier; // 1 or 255?
uniform float pointOpacityPower;
uniform float minPointOpacity;
uniform float maxPointOpacity;

uniform int bPerlin;

in vec4  position;
in vec4  color;
in vec3  normal;

out vec4 colorVarying;

// time passed into the shader each draw
uniform float time;

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

// main program
void main() {
    // make copy of position vector to be able to manipulate it
    vec4 newPosition = position;

    float perlinMultiplier = pow(rand(position.xyz), 3);
    
    if (bPerlin == 1) {
        // perlin noise offset
        float xOffset = noise(vec3(time / 2028., position.x / 729., position.y / 289. + position.x / 890.));
        float yOffset = noise(vec3(position.x / 283. + position.z / 871., time / 3626., position.y / 235.));
        float zOffset = noise(vec3(position.y / 768. + position.z / 652., position.x / 542., time / 1876.));
        xOffset = pow(xOffset / 10., 3);
        yOffset = pow(yOffset / 10., 3);
    //    zOffset = pow(zOffset / 10., 3);
        newPosition.y += yOffset * perlinMultiplier / 4.;
        newPosition.x += xOffset * perlinMultiplier / 4.;
        newPosition.z += zOffset * perlinMultiplier / 6.;
    }
    
    
    // find eye coordinates
    vec4 eyeCoord = modelViewMatrix * newPosition;
    gl_Position = projectionMatrix * eyeCoord;
    
    // distance from eye to point (min = 0; max = 20000)
    float dist = sqrt(eyeCoord.x*eyeCoord.x + eyeCoord.y*eyeCoord.y + eyeCoord.z*eyeCoord.z);
    float clampedDist = clamp(dist, minPointDist, maxPointDist);
    // find normalized distance (0 to 1)
    float normalizedDist = (clampedDist - minPointDist) / (maxPointDist - minPointDist);
    
    // POINT SIZE
    
    // smaller points further away
    float pointSizeScale = pow((1. - normalizedDist), pointSizePower);
    float tempPointSize = pointSizeScale * pointSizeMultiplier;
    float newPtSize = clamp(tempPointSize, minPointSize, maxPointSize);
    
    gl_PointSize = newPtSize;
    
    // POINT OPACITY
    
    // more transparent the farther the point
    float pointOpacityScale = pow((1. - normalizedDist), pointOpacityPower);
    float tempPointOpacity = pointOpacityScale * pointOpacityMultiplier;
    float newPtOpacity = clamp(tempPointOpacity, minPointOpacity, maxPointOpacity);
    
    vec4 newColor = vec4(color.xyz, color.w * newPtOpacity);
    colorVarying = newColor;
}
