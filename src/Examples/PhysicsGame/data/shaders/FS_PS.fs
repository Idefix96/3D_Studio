#version 440

in vec4  Color_FS;
in vec2 UV_FS;

out vec4 FragColor;

uniform sampler2D Sampler;

void main()
{
	FragColor = texture2D(Sampler,UV_FS.xy)*Color_FS;
}