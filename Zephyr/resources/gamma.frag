//#version 330

const float gammaCorrection = 2.2;

vec4 gammaCorrect(in vec4 color) {
    vec4 gamma = vec4(1 / gammaCorrection);
    gamma.w = 1.0;
    return pow(color, gamma);
}
