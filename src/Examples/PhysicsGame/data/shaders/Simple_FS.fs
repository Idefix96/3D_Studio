#version 440
in vec2 UV_FS;
in vec3 Normal_FS;

out vec4 FragColor;

uniform sampler2D Sampler;
uniform vec3 AmbCol;
uniform float AmbInt;
uniform vec3 DirCol;
uniform float DirInt;
uniform vec3 Dir;
uniform vec3 EyePos;
uniform vec3 MeshColor;
uniform float HasTexture;



void main()
{	
	vec3 Normal = Normal_FS;
	
	vec3 AmbientColor = AmbCol * AmbInt;
	
	float DiffuseFactor = dot(Normal, -Dir);
	vec3 DiffuseColor = DirCol * DirInt * DiffuseFactor;
	if (DiffuseFactor < 0)
		DiffuseColor = vec3(0.0,0.0,0.0); 
		
	if (HasTexture > 0)
	{
		vec4 texColor = texture2D(Sampler, UV_FS);
		if (texColor.a < 0.01)
			discard;
		else
		{
			FragColor = vec4(MeshColor.x, MeshColor.y, MeshColor.z, 1.0)*texture2D(Sampler, UV_FS.xy)*vec4(AmbientColor + DiffuseColor ,1.0);
		}
	}
	else
	{
		FragColor = vec4(MeshColor.x, MeshColor.y, MeshColor.z, 1.0)*vec4(AmbientColor + DiffuseColor ,1.0);
	}

}