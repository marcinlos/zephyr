#version 330

const float gammaCorrection = 2.2;

uniform vec4 diffuseColor;
uniform float spec;

in vec3 camPos;
in vec3 worldNorm;
in vec3 camNorm;

layout (std140) uniform CameraMatrices 
{
    mat4 viewMatrix;
    mat4 projectionMatrix;
};

out vec4 outputColor;

uniform vec3 sunDirection;
uniform float sunIntensity;
uniform vec3 sunColor;
uniform vec3 ambient;

uniform float hdrMax;


uniform sampler1D example;

vec4 gammaCorrect(in vec4 color) {
    vec4 gamma = vec4(1 / gammaCorrection);
    gamma.w = 1.0;
    return pow(color, gamma);
}

vec4 normToColor(in vec3 n) {
    float xx = (1 - n.x) / 2;
    float yy = (1 - n.y) / 2;
    float zz = (1 - n.z) / 2;
    
    return vec4(xx, yy, zz, 1);
}

vec3 computeSunlight() {
    float dirFactor = dot(normalize(worldNorm), sunDirection);
    return sunIntensity * clamp(dirFactor, 0, 1) * sunColor;
}

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
    return spec * pow(intensity, 25) * attenuation(dist, lightDir, d);
}


void main()
{
    // Day-night cycle
    vec3 sunComponent = computeSunlight();
    
    vec3 worldN = normalize(worldNorm);
    vec4 nn = normToColor(worldN);
    
    // light    
    vec3 lightPos = vec3(1, 2, 3);
    vec3 lightAt = vec3(-1, -1, 0);
    vec3 lightColor = vec3(1, 1, 1);
    
    lightPos = vec3(viewMatrix * vec4(lightPos, 1));
    lightAt = vec3(viewMatrix * vec4(lightAt, 1));
    
    vec3 lightDir = normalize(lightAt - lightPos);
    vec3 dist = lightPos - camPos;

    float len = length(dist);
    float diffuse = lambert(dist / len, lightDir, length(dist));
    float spec = phong(dist / len, lightDir, length(dist));
    
    // Full light output    
    vec3 total = ambient + sunComponent + diffuse * lightColor;
    vec4 col = diffuseColor * vec4(total, 1) + spec * vec4(1, 1, 1, 1);

    // HDR adjustment
    col = col / hdrMax;
    
    // col = gammaCorrect(col);
    outputColor = nn;//col;// * texture(example, 1.0).r; 
    
    //outputColor = vec4(nn, 1);
}

