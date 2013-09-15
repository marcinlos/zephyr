#version 330

const float gammaCorrection = 2.2;

flat in vec4 diffuseColor;
flat in vec3 normal;

in vec3 worldPos;

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


void main()
{
    vec3 n = normalize(normal);
    
    // Day-night cycle
    vec3 sunComponent = sunIntensity * clamp(dot(n, sunDirection), 0, 1) * sunColor;
    
    // normal color calculation
    float xx = (1 - n.x) / 2;
    float yy = (1 - n.y) / 2;
    float zz = (1 - n.z) / 2;
    
    vec3 nn = vec3(xx, yy, zz);
    
    // light    
    vec3 lightPos = vec3(1, 2, 3);
    vec3 lightAt = vec3(-1, -1, 0);
    vec3 lightColor = vec3(1, 1, 1);
    float coneCutoff = 30;
    
    vec3 lightDir = normalize(lightAt - lightPos);
    
    vec3 dist = lightPos - worldPos;
    vec3 normDist = normalize(dist);
    float angleFactor = clamp(-dot(normDist, n), 0, 1);
    
    float k = clamp(-dot(normDist, lightDir), 0, 1);
    float s = 1 - k * k;
    
    float ss = s * 15;
    float cone = 1 * exp(- ss * ss);
    
    float distanceFactor = 1 / (1 + 0.01 * dot(dist, dist));
    
    float power = angleFactor * distanceFactor * cone;

    // Full light output    
    vec3 total = ambient + clamp(power, 0, 1) * lightColor + sunComponent;
    outputColor = diffuseColor * vec4(total, 1);
    
    outputColor = outputColor / hdrMax;
    //vec4 gamma = vec4(1.0 / gammaCorrection);
    //gamma.w = 1.0;
    //outputColor = pow(outputColor, gamma);
    
}