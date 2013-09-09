#version 330

flat in vec4 theColor;

out vec4 outputColor;

void main()
{
    float lerp = (gl_FragCoord.y - 250.0f) / 300.0f;
    
    vec4 begin = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    vec4 end   = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    
    vec4 overlay = mix(begin, end, lerp);
    outputColor = theColor; //mix(theColor, overlay, 0.0f);
}