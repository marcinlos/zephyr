#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 vertexNormal;

layout (std140) uniform CameraMatrices 
{
    mat4 viewMatrix;
    mat4 projectionMatrix;
};

uniform mat4 modelMatrix;

flat out vec4 diffuseColor;
flat out vec3 normal; 

out vec3 worldPos;


void main() 
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;

    diffuseColor = color;
    normal = vertexNormal;
    
    worldPos = vec3(modelMatrix * position);
}