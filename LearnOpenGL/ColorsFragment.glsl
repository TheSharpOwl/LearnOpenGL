#version 330 core
//adding a material 
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};
struct Light 
{
	vec3 position;
	vec3 direction;
	float cutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

out vec4 FragColor;

uniform vec3 viewPos;//the position of the eye/viewer to calculate the reflection angle

uniform Material material;
uniform Light light;

void main()
{

	///If you're a bit stubborn and still want to set the ambient colors to a different value
	///(other than the diffuse value) you can keep the ambient vec3,
	///but then the ambient colors would still remain the same for the entire object.
	///To get different ambient values for each fragment you'd have to use another
	///texture for ambient values alone.
	vec3 lightDir = normalize(light.position - FragPos);
	float theta = dot(lightDir, normalize(-light.direction));

	if(theta < light.cutOff)//the comparsion sign is reversed because we're comparing cos of the angles
	{			
		// if the angle is not approperiate just use the ambient light so it won't be completely dark
		FragColor = vec4(light.ambient * vec3(texture(material.diffuse, TexCoords)), 1.0);
		return;
	}
	//or do the normal calculations
	vec3 norm = normalize(Normal);
	//max in glsl can compare int and double/float...
	float diff = max(dot(norm,lightDir),0.0);
	//ambient 
	vec3 ambient = light.ambient * (texture(material.diffuse, TexCoords).rgb);
	//diffuse
	//light pos direction is from the surface to the light
	//diffuse becomes more as the angle between the light and the normal becomes smaller
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	//light pos direction is from the surface to the light as I said, so we have to inverse it to get the reflection
	vec3 reflectDir = reflect(-lightDir, norm);//norm is like the mirror
	//This 32 value is the shininess value of the highlight
	float spec = pow(max(dot(viewDir,reflectDir), 0.0), material.shininess);
	vec3 specular  = light.specular * spec * (texture(material.specular, TexCoords).rgb);

	vec3 result = (ambient + diffuse + specular);

	FragColor = vec4(result, 1.0);
}