#version 440

in vec2 UV_FS;
in vec3 Normal_FS;
in vec3 Tangent_FS;
in vec3 Pos_FS;
in vec3 Kd_FS;
in vec4 LightSpacePos;
in mat4 WorldToCamera_FS;

out vec4 FragColor;

uniform sampler2D Shadowmap;
uniform sampler2D Sampler;
uniform sampler2D Normalmap;
uniform vec3 AmbCol;
uniform float AmbInt;
uniform vec3 DirCol;
uniform float DirInt;
uniform vec3 Dir;
uniform vec3 EyePos;
uniform float SpecularIntensity;
uniform float SpecularPower;
uniform vec3 MeshColor;
uniform float HasTexture;

vec3 CalcBumpedNormal()
{
    vec3 Normal = normalize(Normal_FS);
    vec3 Tangent = normalize(Tangent_FS);
    Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
    vec3 Bitangent = cross(Tangent, Normal);
    vec3 BumpMapNormal = texture(Normalmap, UV_FS).xyz;
    BumpMapNormal = 2.0 * BumpMapNormal - vec3(1.0, 1.0, 1.0);
    vec3 NewNormal;
    mat3 TBN = mat3(Tangent, Bitangent, Normal);
    NewNormal = TBN * BumpMapNormal;
    NewNormal = normalize(NewNormal);
    return NewNormal;
}

void main()
{
	vec3 Normal = CalcBumpedNormal();
	
	vec4 tempNewDir = vec4(Dir.x, Dir.y, Dir.z, 0);
	vec3 newDir = vec3(tempNewDir.x, tempNewDir.y, tempNewDir.z);
	
	vec3 AmbientColor = AmbCol * AmbInt;
	float DiffuseFactor = dot(Normal, -newDir);
	vec3 DiffuseColor = DirCol * DirInt * DiffuseFactor;
  
	if (DiffuseFactor < 0)
		DiffuseColor = vec3(0.0,0.0,0.0);    
    
	vec3 VertexToEye = normalize(EyePos - Pos_FS);
	vec3 LightReflect = normalize(reflect(newDir, Normal));
	float SpecularFactor = dot(VertexToEye, LightReflect);
	
	if (SpecularFactor < 0)
		SpecularFactor = 0;
	else 
	{
		SpecularFactor = pow(SpecularFactor, SpecularPower);
	}

	vec3 ProjCoords = LightSpacePos.xyz/LightSpacePos.w;

    vec2 UVCoords;

	float depth = 1.0;
	UVCoords.x = 0.5 * ProjCoords.x + 0.5;
	UVCoords.y = 0.5 * ProjCoords.y + 0.5;
	float z = 0.5 * ProjCoords.z + 0.5;

	vec2 poissonDisk[4] = vec2[](
	vec2( -0.94201624, -0.39906216 ),
	vec2( 0.94558609, -0.76890725 ),
	vec2( -0.094184101, -0.92938870 ),
	vec2( 0.34495938, 0.29387760 )
	);
	
	for (int i=0;i<3;i++)
	{
		float Depth = texture(Shadowmap, vec2(UVCoords.xy + poissonDisk[i]/700.0)).z;
		if ( Depth  <  z-0.005 )
		{
			depth-=0.2;
		}
	}
	
	vec3 SpecularColor = vec3(DirCol * SpecularIntensity * SpecularFactor);
  
	if (HasTexture > 0)
		FragColor = vec4(depth)*texture(Sampler,UV_FS)*vec4(AmbientColor + DiffuseColor + SpecularColor,1.0);
	else
	{
		FragColor = vec4(AmbientColor + DiffuseColor*MeshColor + SpecularColor,1.0);
	}
}