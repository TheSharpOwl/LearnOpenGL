#version 330 core
//adding a material 
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;//the position of the eye/viewer to calculate the reflection angle

uniform Material material;
uniform Light light;

void main()
{
	//ambient 
	vec3 ambient = light.ambient * material.ambient;

	//diffuse
	vec3 norm = normalize(Normal);
	//light pos direction is from the surface to the light
	vec3 lightDir = normalize(lightPos - FragPos);
	//max in glsl can compare int and double/float...
	float diff = max(dot(norm,lightDir),0.0);
	//diffuse becomes more as the angle between the light and the normal becomes smaller
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	//light pos direction is from the surface to the light as I said, so we have to inverse it to get the reflection
	vec3 reflectDir = reflect(-lightDir, norm);//norm is like the mirror
	//This 32 value is the shininess value of the highlight
	float spec = pow(max(dot(viewDir,reflectDir), 0.0), material.shininess);
	vec3 specular  = light.specular * (spec * material.specular);

	vec3 result = (ambient + diffuse + specular);
	FragColor = vec4(result, 1.0);
}