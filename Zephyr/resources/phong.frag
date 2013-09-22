#define GAMMA

in vec3 camPos;

uniform float spec;
uniform float specHardness;


float computeCutoff(vec3 dist, vec3 lightDir, float focus) {
    float k = clamp(-dot(dist, lightDir), 0, 1);
    float s = 1 - k * k;
    
    float ss = s * focus;
    return exp(- ss * ss);
}

float attenuation(float d, float strength) {
    float distanceFactor = 1 / (1 + strength * d * d);
    return distanceFactor;
}

/**
 * @param dir Normalized direction (light soure -> point)
 */
float lambert(vec3 dir, vec3 camNorm) {
    vec3 n = normalize(camNorm);
    float angleFactor = clamp(-dot(dir, n), 0, 1);
    return angleFactor;
}

/**
 * @param lightDir Normalized direction of light
 */
float phong(vec3 dir, vec3 lightDir, vec3 camNorm) {
    vec3 n = normalize(camNorm);
    vec3 toCam = -normalize(vec3(camPos));
    vec3 ref = 2 * dot(dir, n) * n - dir;

    float intensity = clamp(dot(ref, toCam), 0, 1);
    return spec * pow(intensity, specHardness);
}
