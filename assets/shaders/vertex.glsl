#version 330 core

layout (location = 0) in vec3 a_Position;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec4 u_color;

void main()
{
	gl_Position = u_projection * u_view * u_model * vec4(a_Position, 1.0);
	u_color = vec4(a_Position, 1.0);
}
