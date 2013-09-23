//#version 330

uniform vec3 sunDirection;
uniform float sunIntensity;
uniform vec3 sunColor;

/**
 * @param n Worldspace suface normal
 */
vec3 computeSunlight(vec3 n) {
    float dirFactor = dot(-n, sunDirection);
    return sunIntensity * clamp(dirFactor, 0, 1) * sunColor;
}
