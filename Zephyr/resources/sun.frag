#version 330

in vec3 worldNorm;

uniform vec3 sunDirection;
uniform float sunIntensity;
uniform vec3 sunColor;

vec3 computeSunlight() {
    float dirFactor = dot(normalize(worldNorm), sunDirection);
    return sunIntensity * clamp(dirFactor, 0, 1) * sunColor;
}
