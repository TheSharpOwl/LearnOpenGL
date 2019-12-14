#version 330 core
in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;//the position of the eye/viewer to calculate the reflection angle

void main()
{

	vec3 norm = normalize(Normal);
	//light pos direction is from the surface to the light
	vec3 lightDir = normalize(lightPos - FragPos);


	//max in glsl can compare int and double/float...
	float diff = max(dot(norm,lightDir),0.0);


	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	//light pos direction is from the surface to the light as I said, so we have to inverse it to get the reflection
	vec3 reflectDir = reflect(-lightDir, norm);//norm is like the mirror
	//This 32 value is the shininess value of the highlight
	float spec = pow(max(dot(viewDir,reflectDir), 0.0), 32);
	vec3 specular  = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}