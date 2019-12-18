#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;//to be passed to the fragment shader
layout (location = 2) in vec2 aTexCoords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;//to pass the normal coordinates to the fragment shader
out vec2 TexCoords;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	FragPos = vec3(model * vec4(aPos, 1.0));
	//This formula gives multiplication with the Normal matrix because some translations fuck up the normal vectors and they won't be prependicular to the surface anymore
	Normal = mat3(transpose(inverse(model))) * aNormal;
	TexCoords = aTexCoords;
}