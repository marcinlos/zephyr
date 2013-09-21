#define GAMMA

in vec3 camNorm;
in vec3 camPos;
uniform float spec;
uniform float specHardness;


float computeCutoff(in vec3 dist, in vec3 lightDir) {
    float k = clamp(-dot(dist, lightDir), 0, 1);
    float s = 1 - k * k;
    
    float ss = s * 15;
    return 1 * exp(- ss * ss);
}

float attenuation(in vec3 dist, in vec3 lightDir, float d) {
    float cutoff = computeCutoff(dist, lightDir);
    float distanceFactor = 1 / (1 + 0.1 * d * d);
    return distanceFactor * cutoff;
}

float lambert(in vec3 dist, in vec3 lightDir, float d) {
    vec3 n = normalize(camNorm);
    float angleFactor = clamp(-dot(dist, n), 0, 1);
    return angleFactor * attenuation(dist, lightDir, d);
}

float phong(in vec3 dist, in vec3 lightDir, float d) {
    vec3 n = normalize(camNorm);
    vec3 toCam = -normalize(vec3(camPos));
    vec3 ref = 2 * dot(dist, n) * n - dist;

    float intensity = clamp(dot(ref, toCam), 0, 1);
    return spec * pow(intensity, specHardness) * attenuation(dist, lightDir, d);
}
