//#version 330

const float gammaCorrection = 2.2;

vec3 gammaCorrect(vec3 color) {
    vec3 gamma = vec3(1 / gammaCorrection);
    return pow(color, gamma);
}
