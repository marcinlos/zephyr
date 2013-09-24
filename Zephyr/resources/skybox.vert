
layout(location = 0) in vec3 position;

layout (std140) uniform CameraMatrices 
{
    mat4 viewMatrix;
    mat4 projectionMatrix;
};

out vec3 coords;

void main() {
    vec4 pos = projectionMatrix * viewMatrix * vec4(position, 0);
    gl_Position = pos.xyww;
    coords = position;
}
