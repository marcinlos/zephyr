#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

layout (std140) uniform CameraMatrices 
{
    mat4 viewMatrix;
    mat4 projectionMatrix;
};

uniform mat4 modelMatrix;

smooth out vec4 theColor;


void main() 
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
    vec3 p = vec3(gl_Position);
    p.z = log(1 + p.z / 30);
    float s = 3 / (0.2 + dot(p, p));
    
    vec4 light = vec4(1, 1, 1, 1);
    
    theColor = 0.9f * color + (light * color + 0.1 * light) * s;
    //theColor = color;
}