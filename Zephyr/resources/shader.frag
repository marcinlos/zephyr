#version 330

flat in vec4 diffuseColor;
flat in vec3 normal;

out vec4 outputColor;

uniform vec3 sunDirection;
uniform float sunIntensity;

uniform float ambient;


void main()
{
    float sunComponent = sunIntensity * clamp(dot(normal, sunDirection), 0, 1);
    
    float intensity = ambient + sunComponent;
    
    //float selfGlow = 
    outputColor = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0f);
    outputColor = diffuseColor * clamp(intensity, 0, 1);
}