#version 330 core
out vec4 FragColor;

in vec3 MyColor;

void main()
{
	FragColor = vec4(MyColor, 1.0f);
}