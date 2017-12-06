#version 440

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec3 Kd;

out vec3 Color_FS;
out vec2 UV_FS;
out vec3 Normal_FS;
out vec3 Tangent_FS;
out vec3 Pos_FS;
out vec3 Kd_FS;
out mat4 WorldToCamera_FS;
out vec4 LightSpacePos;

uniform mat4 gPersp;
uniform mat4 gLightPersp;
uniform mat4 gWorldToCamera;
uniform mat4 gScale;
uniform mat4 gRotationX;
uniform mat4 gRotationY;
uniform mat4 gRotationZ;
uniform mat4 gTranslation;
uniform mat4 gLightWVP;
void main()
{
	UV_FS = UV;
	Kd_FS = Kd;
	WorldToCamera_FS = gWorldToCamera;
 	LightSpacePos = gLightPersp*gLightWVP*gTranslation*gRotationZ*gRotationY*gRotationX*gScale*vec4(Position, 1.0);
	vec4 temp_normal = gRotationZ*gRotationY*gRotationX * vec4(Normal.x, Normal.y, Normal.z, 0.0);
	Normal_FS = vec3(temp_normal.x, temp_normal.y, temp_normal.z);
	vec4 temp_tangent = gRotationZ*gRotationY*gRotationX * vec4(Tangent.x, Tangent.y, Tangent.z, 0.0);
	Tangent_FS = vec3(temp_tangent.x, temp_tangent.y, temp_tangent.z);
	vec4 Pos_FS_temp = gTranslation*gRotationZ*gRotationY*gRotationX*gScale*vec4(Position.x, Position.y, Position.z, 1.0);
	Pos_FS = vec3(Pos_FS_temp.x,Pos_FS_temp.y,Pos_FS_temp.z);
	gl_Position = gPersp*gWorldToCamera*gTranslation*gRotationZ*gRotationY*gRotationX*gScale*vec4(Position.x, Position.y, Position.z, 1.0);
}