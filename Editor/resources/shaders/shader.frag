#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float mixVal;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec4 t1 = texture2D(texture1, TexCoord);
    vec4 t2 = texture2D(texture2, TexCoord);
    FragColor = mix(t1, t2, mixVal);
    //FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 1.0);
}