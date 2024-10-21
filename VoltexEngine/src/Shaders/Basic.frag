#version 150 core

in vec2 TexCoords;

out vec4 outColor;

uniform vec4 color;
uniform sampler2D tex;

void main()
{
	outColor = texture(tex, TexCoords) * color;
}