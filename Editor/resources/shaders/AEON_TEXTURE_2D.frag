#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform float opacity;

// texture sampler
uniform sampler2D texture1;

void main()
{
	vec4 texColor = texture(texture1, TexCoord);

	// if(texColor.a < 0.1) discard;
	if(texColor.a < 0.1 || opacity == 0) discard;

	 // Multiply the alpha component of the texture color by the opacity
    texColor.a *= opacity;		// added for opacity 
	FragColor = texColor;
}