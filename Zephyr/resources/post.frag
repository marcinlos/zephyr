
in vec2 uv;

out vec3 color;

uniform sampler2D renderedTexture;

void main() {
    color = texture(renderedTexture, uv).xyz;
}
