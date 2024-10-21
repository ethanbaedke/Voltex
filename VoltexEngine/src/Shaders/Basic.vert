#version 150 core

in vec2 inPosition;
in vec2 inTexCoords;

out vec2 TexCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(inPosition.x, inPosition.y, 0.0, 1.0);
	TexCoords = inTexCoords;
}