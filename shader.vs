#version 330

layout (location = 0) in vec3 Position;
uniform float colorChange;
out vec4 Color; 

void main()
{
    gl_Position = vec4(0.5 * Position.x, 0.5 * Position.y, Position.z, 1.0);
	Color = vec4(Position.x + colorChange, Position.y + 2 * colorChange, Position.z + 3 * colorChange, 1.0);
}