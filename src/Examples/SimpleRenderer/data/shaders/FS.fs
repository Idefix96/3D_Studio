#version 440

in vec2 UV_FS;
in vec3 Normal_FS;
in vec3 Pos_FS;
in vec3 Tangent_FS;
in mat4 WorldToCamera_FS;

out vec4 FragColor;

uniform sampler2D Sampler;
uniform sampler2D Normalmap;
uniform sampler2D Shadowmap;
uniform sampler2D Shadowmap_middle;
uniform sampler2D Shadowmap_far;
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
uniform float HasNormalMap;
uniform mat4 gLightWVP;
uniform mat4 gLightWVP2;

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
	vec3 Normal;
	if (HasNormalMap > 0)
		Normal = normalize(CalcBumpedNormal());
	else
		Normal = Normal_FS;

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
  
	vec3 SpecularColor = vec3(DirCol.x * SpecularIntensity * SpecularFactor, DirCol.y * SpecularIntensity * SpecularFactor, DirCol.z * SpecularIntensity * SpecularFactor);
  
	float visibility = 1.0;
	vec4 ShadowCoord = gLightWVP2*vec4(Pos_FS, 1.0);
	
	if ( (ShadowCoord.x >= 0)  && (ShadowCoord.y >= 0))
	 if ( (ShadowCoord.x <= 1)  && (ShadowCoord.y <= 1))
		if ( texture( Shadowmap, ShadowCoord.xy ).z  <  ShadowCoord.z - 0.005)
		{
			visibility = 0.1;
		}
	
	if (HasTexture > 0)
	{
		vec4 texColor = texture2D(Sampler, UV_FS);
		if (texColor.a < 0.01)
			discard;
		else
		{
			FragColor = vec4(MeshColor.x, MeshColor.y, MeshColor.z, 1.0)*texture2D(Sampler, UV_FS.xy)*vec4(AmbientColor + visibility*DiffuseColor + visibility*SpecularColor,1.0);
		}
	}
	else
	{
		FragColor = vec4(MeshColor.x, MeshColor.y, MeshColor.z, 1.0)*vec4(AmbientColor + visibility*DiffuseColor + visibility*SpecularColor,1.0);
	}
	
}