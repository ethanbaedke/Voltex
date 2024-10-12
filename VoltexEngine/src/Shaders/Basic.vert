#version 150 core

in vec2 inPosition;
in vec3 inColor;
in vec2 inTexCoords;

out vec3 Color;
out vec2 TexCoords;

void main()
{
	gl_Position = vec4(inPosition.x, inPosition.y, 0.0, 1.0);
	Color = inColor;
	TexCoords = inTexCoords;
}