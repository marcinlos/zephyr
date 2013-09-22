
in vec2 uv;

out vec3 outputColor;

uniform sampler2D renderedTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;
uniform sampler2D depthTexture;

vec3 normToColor(vec3 n);

uniform uvec4 viewport;


uniform bool blurActive;
uniform vec2 blurDir;
uniform float blurStrength;
const int samples = 3;

float blurCoeff[10] = float[](
    1.0f, 0.9f, 0.7f, 0.4f, 0.3f, 0.2f, 0.15f, 0.1f, 0.07f, 0.03f
);

vec3 computeMotionBlur(vec3 color) {
    if (blurActive) {
        vec2 begin = uv + blurStrength * blurDir;
        
        vec3 blurColor = vec3(0);
        float norm = 0.0f;
        for (int i = 0; i < samples; ++ i) {
            float a = i / float(samples);
            vec2 pos = mix(begin, uv, a);
            float coeff = blurCoeff[i];
            norm += coeff;
            blurColor += coeff * texture(renderedTexture, pos).rgb;
        }   
        blurColor /= norm;
        return mix(blurColor, color, 0.7f);
    } else {
        return color;
    }
}

void main() {
    vec3 color = texture(renderedTexture, uv).rgb;
    vec3 normal = texture(normalTexture, uv).xyz;
    vec3 specular = texture(specularTexture, uv).rgb;
    float depth = texture(depthTexture, uv).r;

    outputColor = vec3(depth);
    outputColor = specular;
    
    float x = gl_FragCoord.x;
    float y = gl_FragCoord.y;

    vec2 d = 2.0 * gl_FragCoord.xy - viewport.zw;

    d /= viewport.zw;
    
    float r2 = length(d);
    float a = 1 - pow(r2 / 1.3, 5) / 2;
    outputColor = mix(vec3(0, 0, 0), computeMotionBlur(color), a);

}


