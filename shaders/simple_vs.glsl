#version 330 core

layout(location = 0) in vec2 Position;

uniform float itime;
uniform float x;
uniform float y;

void	main()
{
	mat2 mat = mat2(cos(itime), -sin(itime),
					sin(itime),  cos(itime));
	gl_Position = vec4(vec2(Position.x + x, Position.y + y) * mat, 0.0f, 1.0f);
}

