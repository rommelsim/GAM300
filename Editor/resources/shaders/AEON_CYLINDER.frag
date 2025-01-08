#version 330 core

in vec3 FragPos;
in vec3 Normal;

// Out
out vec4 fragColor;

uniform vec3 lightPos;			//  Position of light src
uniform vec3 lightColor;		//  Color of light
uniform vec3 color;             //  Object colour

uniform int lightMode;          //  1 - On, 0 - Off

void main()
{
    switch(lightMode)
    {
    case 1:
        // Compute lighting
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float ambientStr = 0.1;
        vec3 ambient = ambientStr * lightColor;
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        // Combine lighting with texture 
        vec3 result = (ambient + diffuse) * color;

        // Output final color
        fragColor = vec4(result, 1.0f);
        break;
        
    default:
        fragColor = vec4(color, 1.0f);
    }
}