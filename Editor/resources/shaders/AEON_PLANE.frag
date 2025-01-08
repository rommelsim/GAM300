#version 330 core

in vec2 Texture_Pos;
in vec3 Normal;
in vec3 FragPos;

out vec4 fragColor;

uniform sampler2D Texture_ID;	// Texture used on current object
uniform vec3 lightPos;			// Position of light src
uniform vec3 lightColor;		// Color of light
uniform vec3 objectColor;		// Current object color

uniform vec3 color;

uniform int lightMode;

void main()
{
	// switch(lightMode)
	// {
	// 	case 1:
	// 		// Compute lighting
	// 		vec3 norm = normalize(Normal);
	// 		vec3 lightDir = normalize(lightPos - FragPos);
	// 		float ambientStr = 0.1;
	// 		vec3 ambient = ambientStr * lightColor;
	// 		float diff = max(dot(norm, lightDir), 0.0);
	// 		vec3 diffuse = diff * lightColor;

	// 		// Compute final color using Texture
	// 		vec4 texColor = texture(Texture_ID, Texture_Pos);

	// 		// Combine lighting with texture 
	// 		vec3 result = (ambient + diffuse) * objectColor.rgb;

	// 		// Output final color
	// 		fragColor = vec4(result * texColor.rgb, texColor.a);
	// 		break;
		
	// 	default:
	// 		//texColor = texture(Texture_ID, Texture_Pos);
	// 		fragColor = texColor;
	// 		break;
	// }

	fragColor = vec4(color, 1.0f);
	
}