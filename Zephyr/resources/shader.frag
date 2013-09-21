//#version 330


uniform vec4 diffuseColor;

in vec3 camPos;
in vec3 worldNorm;
in vec3 camNorm;
in vec2 texCoord;

layout (std140) uniform CameraMatrices 
{
    mat4 viewMatrix;
    mat4 projectionMatrix;
};

uniform vec3 ambient;
uniform float hdrMax;
uniform sampler2D example;

out vec4 outputColor;

vec3 computeSunlight();

float lambert(in vec3 dist, in vec3 lightDir, float d);
float phong(in vec3 dist, in vec3 lightDir, float d);

// light    
vec3 lightPos = vec3(1, 2, 3);
vec3 lightAt = vec3(-1, -1, 0);
vec3 lightColor = vec3(1, 1, 1);

vec4 gammaCorrect(in vec4 color);

vec4 make = diffuseColor;

void main()
{
    // Day-night cycle
    vec3 sunComponent = computeSunlight();
    
    lightPos = vec3(viewMatrix * vec4(lightPos, 1));
    lightAt = vec3(viewMatrix * vec4(lightAt, 1));
    
    vec3 lightDir = normalize(lightAt - lightPos);
    vec3 dist = lightPos - camPos;

    float len = length(dist);
    float diffuse = lambert(dist / len, lightDir, length(dist));
    float spec = phong(dist / len, lightDir, length(dist));

    vec4 specColor = vec4(1, 1, 1, 1);

    
    // Full light output    
    vec3 total = ambient + sunComponent + diffuse * lightColor;
    vec4 col = texture(example, texCoord) * vec4(total, 1) + spec * specColor;
    // HDR adjustment
    col = col / hdrMax;
    
#ifdef GAMMA
    col = gammaCorrect(col);
#endif

    outputColor = col; 
}

