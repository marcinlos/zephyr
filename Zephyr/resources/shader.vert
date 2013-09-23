//#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in vec3 inTangent;
layout(location = 5) in vec3 inBitangent;

layout (std140) uniform CameraMatrices 
{
    mat4 viewMatrix;
    mat4 projectionMatrix;
};

uniform mat4 modelMatrix;

out vec4 diffuseColor;
out vec3 normal; 
out vec2 texCoord;

out vec3 camPos;
out vec3 worldNorm;
out vec3 camNorm;

out vec3 tangent;
out vec3 bitangent;


void main() 
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;

    diffuseColor = color;
    
    vec4 hn = vec4(vertexNormal, 0);
    
    camPos = vec3(viewMatrix * modelMatrix * position);
    worldNorm = vec3(modelMatrix * hn);
    camNorm = vec3(viewMatrix * modelMatrix * hn);
    texCoord = inTexCoord;

    /*tangent = inTangent;
    bitangent = outBitangent;*/
}
