//#version 330


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
uniform sampler2D diffuseTexture;

uniform uvec4 viewport;

uniform vec4 specColor;

#ifdef DIFFUSE_UNIFORM
    uniform vec4 diffuseColor;
#elif defined DIFFUSE_TEXTURE
    vec4 diffuseColor = texture(diffuseTexture, texCoord);
#endif    

//out vec4 outputColor;
layout(location = 0) out vec3 outputColor;

vec3 computeSunlight();
		
float lambert(vec3 dir);
float phong(vec3 dir, vec3 lightDir);

float attenuation(float d, float strength);
float computeCutoff(vec3 dist, vec3 lightDir, float focus);

// light    
vec3 lightPos = vec3(1, 2, 3);
vec3 lightAt = vec3(-1, -1, 0);
vec3 lightColor = vec3(1, 1, 1);
float lightAtten = 0.1;
float lightFocus = 15;

#ifdef GAMMA
    vec4 gammaCorrect(vec4 color);
#endif

vec4 windowToNdc(vec2 xy) {
    vec4 ndcPos;
    ndcPos.xy = ((2.0 * xy) - (2u * viewport.xy)) / (viewport.zw) - 1;
    ndcPos.z = (2.0 * gl_FragCoord.z - gl_DepthRange.near - gl_DepthRange.far) /
        (gl_DepthRange.far - gl_DepthRange.near);
    ndcPos.w = 1.0;
    return ndcPos;
}


void main()
{
    // Day-night cycle
    vec3 sunComponent = computeSunlight();
    
    lightPos = vec3(viewMatrix * vec4(lightPos, 1));
    lightAt = vec3(viewMatrix * vec4(lightAt, 1));
    
    vec3 lightDir = normalize(lightAt - lightPos);
    vec3 dist = lightPos - camPos;

    float len = length(dist);
    vec3 unit = dist / len;

    float atten = attenuation(len, lightAtten);
    float cutoff = computeCutoff(unit, lightDir, lightFocus);
    float diffuse = atten * cutoff * lambert(unit);
    float spec = atten * cutoff * phong(unit, lightDir);

    vec4 specColor = vec4(1, 1, 1, 1);

    
    // Full light output    
    vec3 total = ambient + sunComponent + diffuse * lightColor;
    vec4 col = diffuseColor * vec4(total, 1) + spec * specColor;
    // HDR adjustment
    col = col / hdrMax;
    
#ifdef GAMMA
    col = gammaCorrect(col);
#endif

    outputColor = vec3(col);

    /*
    vec4 ndcPos = windowToNdc(gl_FragCoord.xy);
    outputColor = col;
    float depth = 0.5 * ndcPos.z + 0.5;
    outputColor = mix(col, vec4(1.0), pow(depth, 5));
    */
}

