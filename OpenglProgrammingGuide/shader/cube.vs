#version 430 core

layout (location = 0) in vec4 v_position;
layout (location = 1) in vec4 v_color;

out vec4 Color;
uniform mat4 gWorld; 

void main()
{
	gl_Position = vec4(gWorld * v_position);
	Color = v_color;
}