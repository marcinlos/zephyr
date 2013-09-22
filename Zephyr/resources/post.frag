
in vec2 uv;

out vec3 outputColor;

uniform sampler2D renderedTexture;
uniform sampler2D normalTexture;
uniform sampler2D specularTexture;
uniform sampler2D depthTexture;

vec3 normToColor(vec3 n);


uniform bool blurActive;
uniform vec2 blurDir;
uniform float blurStrength;
const int samples = 10;

float blurCoeff[10] = float[](
    1.0f, 0.9f, 0.7f, 0.4f, 0.3f, 0.2f, 0.15f, 0.1f, 0.07f, 0.03f
);


void main() {
    vec3 color = texture(renderedTexture, uv).rgb;
    vec3 normal = texture(normalTexture, uv).xyz;
    vec3 specular = texture(specularTexture, uv).rgb;
    float depth = texture(depthTexture, uv).r;

    outputColor = normToColor(normal);
    outputColor = vec3(depth);
    outputColor = specular;

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
        outputColor = mix(blurColor, color, 0.3f);
    } else {
        outputColor = color;
    }
    //outputColor = color;
}
