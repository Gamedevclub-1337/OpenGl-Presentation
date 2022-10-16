#version 330 core

out	vec4	color;

uniform float colorr;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void	main()
{
	vec2 v = vec2(10., 10.);
	color = vec4(rand(v * colorr), smoothstep(rand(v * colorr), 10., 0.3), rand(v * colorr), 1.0f);
}
