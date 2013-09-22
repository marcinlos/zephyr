//#version 330

uniform vec3 sunDirection;
uniform float sunIntensity;
uniform vec3 sunColor;

vec3 computeSunlight(vec3 worldNormal) {
    float dirFactor = dot(normalize(worldNormal), sunDirection);
    return sunIntensity * clamp(dirFactor, 0, 1) * sunColor;
}
