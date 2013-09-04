#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

smooth out vec4 theColor;

uniform vec2 offset;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 perspectiveMatrix;


void main() 
{
    gl_Position = perspectiveMatrix * viewMatrix * modelMatrix * position;
    theColor = color;
}