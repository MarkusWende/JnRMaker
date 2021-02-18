#version 410 core
layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

flat out vec3 startPos;
out vec3 vertPos;

void main()
{
	vec4 pos = projection * view * model * vec4(aPos, 1.0);
	gl_Position = pos;
	vertPos     = pos.xyz / pos.w;
    startPos    = vertPos;
}