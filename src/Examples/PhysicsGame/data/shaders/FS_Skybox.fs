#version 440

in vec2 UV_FS;

out vec4 FragColor;

uniform sampler2D Sampler;
uniform vec3 MeshColor;
uniform float HasTexture;

void main()
{

	if (HasTexture > 0)
	{
		vec4 texColor = texture2D(Sampler, UV_FS);
		if (texColor.a < 0.1)
			discard;
		else
		{
			FragColor = texture2D(Sampler, UV_FS.xy);
		}
	}
	else
	{
		FragColor = vec4(MeshColor.x, MeshColor.y, MeshColor.z, 1.0);
	}
}