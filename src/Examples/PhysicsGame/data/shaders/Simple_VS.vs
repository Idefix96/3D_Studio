#version 440

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec3 Normal;

out vec2 UV_FS;
out vec3 Normal_FS;

uniform mat4 gPersp;
uniform mat4 gWorldToCamera;
uniform mat4 gScale;
uniform mat4 gRotationX;
uniform mat4 gRotationY;
uniform mat4 gRotationZ;
uniform mat4 gTranslation;

void main()
{
	UV_FS = UV;
	vec4 temp_normal = gRotationZ*gRotationY*gRotationX *vec4(Normal.x, Normal.y, Normal.z, 0.0);
	Normal_FS = vec3(temp_normal.x, temp_normal.y, temp_normal.z);
	
	gl_Position = gPersp*gWorldToCamera*gTranslation*gRotationZ*gRotationY*gRotationX*gScale*vec4(Position.x, Position.y, Position.z, 1.0);

}