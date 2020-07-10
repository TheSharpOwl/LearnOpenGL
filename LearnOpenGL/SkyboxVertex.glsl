#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	TexCoords = aPos;
	// Here I want to render the skybox after the box so I draw it after but I want to also convince the depth 
	// tester that it has z = 1 so we set z = w (because it will be divided by w)
	vec4 pos = projection * view * vec4(aPos, 1.0);
	// careful xyww not xyzw
	gl_Position = pos.xyww;
}