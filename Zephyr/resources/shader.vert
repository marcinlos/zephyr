#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;

uniform vec2 offset;

smooth out vec4 theColor;

void main() 
{
    vec4 offset4 = vec4(offset.x, offset.y, 0, 0);
    gl_Position = position + offset4;
    theColor = color;
}