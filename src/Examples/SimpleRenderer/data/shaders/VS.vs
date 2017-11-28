#version 440

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec4 BoneID;
layout (location = 5) in vec4 BoneWeight;
layout (location = 6) in vec4 BoneID2;
layout (location = 7) in vec4 BoneWeight2;
layout (location = 8) in vec4 BoneID3;
layout (location = 9) in vec4 BoneWeight3;
layout (location = 10) in vec4 BoneID4;
layout (location = 11) in vec4 BoneWeight4;
out vec2 UV_FS;
out vec3 Normal_FS;
out vec3 Pos_FS;
out vec3 Tangent_FS;
out mat4 WorldToCamera_FS;

const int MAX_BONES = 200;

uniform mat4 gPersp;
uniform mat4 gWorldToCamera;
uniform mat4 gScale;
uniform mat4 gRotationX;
uniform mat4 gRotationY;
uniform mat4 gRotationZ;
uniform mat4 gTranslation;
uniform mat4 gBoneTransformation[MAX_BONES];
uniform mat4 gLightWVP;

void main()
{
	UV_FS = UV;
	WorldToCamera_FS = gWorldToCamera;
	
	vec4 bwn = BoneWeight;
	
	mat4 BoneTransform = gBoneTransformation[int(BoneID[0])]*bwn.x;
	BoneTransform += gBoneTransformation[int(BoneID[1])]*bwn.y;
	BoneTransform += gBoneTransformation[int(BoneID[2])]*bwn.z;
	BoneTransform += gBoneTransformation[int(BoneID[3])]*bwn.w;
	
	if (BoneWeight.x+BoneWeight.y+BoneWeight.z+BoneWeight.w == 0)
		BoneTransform = mat4(1.0);
		
	vec4 temp_normal = gTranslation*gRotationZ*gRotationY*gRotationX * BoneTransform*vec4(Normal.x, Normal.y, Normal.z, 0.0);
	Normal_FS = vec3(temp_normal.x, temp_normal.y, temp_normal.z);
	vec4 temp_tangent = gRotationZ*gRotationY*gRotationX * BoneTransform*vec4(Tangent.x, Tangent.y, Tangent.z, 0.0);
	Tangent_FS = vec3(temp_tangent.x, temp_tangent.y, temp_tangent.z);
	vec4 Pos_FS_temp = gTranslation*gRotationZ*gRotationY*gRotationX*gScale*BoneTransform*vec4(Position.x, Position.y, Position.z, 1.0);
	Pos_FS = vec3(Pos_FS_temp.x,Pos_FS_temp.y,Pos_FS_temp.z);

	gl_Position = gPersp*gWorldToCamera*gTranslation*gRotationZ*gRotationY*gRotationX*gScale*BoneTransform*vec4(Position.x, Position.y, Position.z, 1.0);
}