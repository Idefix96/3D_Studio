#version 440

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 Tangent;

out vec2 UV_FS;
out vec3 Normal_FS;
out vec3 Pos_FS;
out vec3 Tangent_FS;
out mat4 WorldToCamera_FS;

void main()
{
	UV_FS = UV;
	WorldToCamera_FS = mat4(1.0);

	vec4 temp_normal = vec4(Normal.x, Normal.y, Normal.z, 0.0);
	Normal_FS = vec3(temp_normal.x, temp_normal.y, temp_normal.z);
	vec4 temp_tangent = vec4(Tangent.x, Tangent.y, Tangent.z, 0.0);
	Tangent_FS = vec3(temp_tangent.x, temp_tangent.y, temp_tangent.z);
	vec4 Pos_FS_temp = vec4(Position.x, Position.y, Position.z, 1.0);
	Pos_FS = vec3(Pos_FS_temp.x,Pos_FS_temp.y,Pos_FS_temp.z);
			
	gl_Position = vec4(Position.x, Position.y, Position.z, 1.0);
}