#version 330

flat in vec4 diffuseColor;
flat in vec3 normal;

layout (std140) uniform CameraMatrices 
{
    mat4 viewMatrix;
    mat4 projectionMatrix;
};

out vec4 outputColor;

uniform vec3 sunDirection;
uniform float sunIntensity;

uniform float ambient;


void main()
{
    vec3 n = normal;
    float sunComponent = sunIntensity * clamp(dot(n, sunDirection), 0, 1);
    
    float intensity = ambient + sunComponent;
    
    
    float xx = (1 + n.x) / 2;
    float yy = (1 + n.y) / 2;
    float zz = (1 - n.z) / 2;
    
    vec3 nn = vec3(zz, zz, zz);
    outputColor = vec4(nn, 1);//diffuseColor * clamp(intensity, 0, 1);
    outputColor = diffuseColor;
}