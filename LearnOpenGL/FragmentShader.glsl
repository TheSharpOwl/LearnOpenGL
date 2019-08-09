#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float MixValue;

void main()
{
	// the second argument is 0.2 because we want 80% of the first argument and 20% of the second argument
	FragColor = mix(texture(texture1, TexCoord) ,texture(texture2, TexCoord), MixValue);
}