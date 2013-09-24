
in vec3 coords;

out vec4 color;


uniform vec3 sunDirection;
uniform float sunIntensity;
uniform vec3 sunColor;
uniform float timeOfDay;

layout (std140) uniform CameraMatrices 
{
    mat4 viewMatrix;
    mat4 projectionMatrix;
};

uniform samplerCube cubeTex;
uniform vec3 sunDir;

void main() {
    vec3 skyColor = 0.3 * vec3(0.6f, 1.0f, 2.0f);

    vec3 sunPos = -sunDirection;
    vec3 dist = sunPos - normalize(coords);
    float diff = 1.0f / (0.2f + 15 * length(dist));

    vec3 final = skyColor + diff * sunColor;//texture(cubeTex, coords);
    color = vec4(final, 1);
}


