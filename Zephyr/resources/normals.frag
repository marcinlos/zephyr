#version 330

vec4 normToColor(in vec3 n) {
    float xx = (1 - n.x) / 2;
    float yy = (1 - n.y) / 2;
    float zz = (1 - n.z) / 2;
    
    return vec4(xx, yy, zz, 1);
}
