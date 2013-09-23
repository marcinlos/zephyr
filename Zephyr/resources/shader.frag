//#version 330


in vec3 camPos;
in vec3 worldNorm;
//in vec3 camNorm;
in vec2 texCoord;
in vec3 tangent;
in vec3 bitangent;

layout (std140) uniform CameraMatrices 
{
    mat4 viewMatrix;
    mat4 projectionMatrix;
};

uniform vec3 ambient;
uniform float hdrMax;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

uniform uvec4 viewport;

vec3 specColor = vec3(1, 1, 1);
uniform float spec;

#ifdef DIFFUSE_UNIFORM
    uniform vec4 diffuseColor;
#elif defined DIFFUSE_TEXTURE
    vec4 diffuseColor = texture(diffuseTexture, texCoord);
#endif    

vec3 worldNormal = worldNorm;

uniform bool useBumpMap;

layout(location = 0) out vec3 outputColor;
layout(location = 1) out vec3 outputNormal;
layout(location = 2) out vec4 outputSpecular;
layout(location = 3) out float outputDepth;

vec3 computeSunlight(vec3 n);
		
float lambert(vec3 dir, vec3 camNorm);
float phong(vec3 dir, vec3 lightDir, vec3 camNorm);

float attenuation(float d, float strength);
float computeCutoff(vec3 dist, vec3 lightDir, float focus);

// light    
uniform vec3 lightPos;
uniform vec3 lightAt;


//vec3 lightPos = vec3(1, 2, 3);
//vec3 lightAt = vec3(-1, -1, 0);
vec3 lightColor = vec3(1, 1, 1);
float lightAtten = 0.05;
float lightFocus = 10;

#ifdef GAMMA
vec3 gammaCorrect(vec3 color);
#endif

vec4 windowToNdc(vec2 xy) {
    vec4 ndcPos;
    ndcPos.xy = ((2.0 * xy) - (2u * viewport.xy)) / (viewport.zw) - 1;
    ndcPos.z = (2.0 * gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) /
        (gl_DepthRange.far - gl_DepthRange.near);
    ndcPos.w = 1.0;
    return ndcPos;
}

void checkBumpMap() {
    if (useBumpMap) {
        vec3 n = normalize(worldNorm);
        vec3 t = normalize(tangent);
        vec3 b = normalize(bitangent);

        vec3 texNorm = 2 * texture(normalTexture, texCoord).xyz - 1.0;
        worldNormal = normalize(mat3(t, b, n) * texNorm);
    }
}


void main()
{
    checkBumpMap();
    vec3 camNorm = vec3(viewMatrix * vec4(worldNormal, 0));

    // Day-night cycle
    vec3 sunComponent = computeSunlight(worldNormal);
    
    vec3 camLightPos = vec3(viewMatrix * vec4(lightPos, 1));
    vec3 camLightAt = vec3(viewMatrix * vec4(lightAt, 1));
    
    vec3 lightDir = -normalize(camLightPos - camLightAt);
    vec3 dist = camLightPos - camPos;

    float len = length(dist);
    vec3 unit = dist / len;

    float atten = attenuation(len, lightAtten);
    float cutoff = computeCutoff(unit, lightDir, lightFocus);
    float diffuse = atten * cutoff * lambert(unit, camNorm);
    float specular = atten * cutoff * phong(unit, lightDir, camNorm);

    // Full light output    
    vec3 total = ambient + sunComponent + diffuse * lightColor;
    vec3 col = vec3(diffuseColor) * total + specular * specColor;
    // HDR adjustment
    col = col / hdrMax;
    
#ifdef GAMMA
    col = gammaCorrect(col);
#endif

    //outputColor = vec3(diffuseColor);
    outputColor = col.rgb;

    outputNormal = normalize(worldNormal);

    outputSpecular = vec4(specColor, spec);

    vec4 ndcPos = windowToNdc(gl_FragCoord.xy);
    float depth = 0.5 * ndcPos.z + 0.5;
    outputDepth = depth;
}

