#version 440

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 UV;
layout (location = 2) in vec3 Normal;
layout (location = 3) in vec3 Tangent;
layout (location = 4) in vec4 BoneID;
layout (location = 5) in vec4 BoneWeight;

const int MAX_BONES = 100;

uniform mat4 gScale;
uniform mat4 gRotationX;
uniform mat4 gRotationY;
uniform mat4 gRotationZ;
uniform mat4 gTranslation;
uniform mat4 gLightWVP;
uniform mat4 gBoneTransformation[MAX_BONES];

void main()
{
	mat4 BoneTransform = gBoneTransformation[int(BoneID[0])]*BoneWeight.x;
	BoneTransform += gBoneTransformation[int(BoneID[1])]*BoneWeight.y;
	BoneTransform += gBoneTransformation[int(BoneID[2])]*BoneWeight.z;
	BoneTransform += gBoneTransformation[int(BoneID[3])]*BoneWeight.w;

	if (BoneWeight.x+BoneWeight.y+BoneWeight.z+BoneWeight.w == 0)
		BoneTransform = mat4(1.0);

	gl_Position = gLightWVP*gTranslation*gRotationZ*gRotationY*gRotationX*gScale*BoneTransform*vec4(Position.x, Position.y, Position.z, 1.0);
}