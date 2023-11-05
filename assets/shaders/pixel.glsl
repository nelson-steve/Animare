#version 330 core

layout(location = 0) out vec4 color;

uniform vec3 u_color;

void main()
{
	//color = u_color;
	vec3 white = vec3(1.0, 1.0, 1.0);
	color = vec4(white, 1.0);
}