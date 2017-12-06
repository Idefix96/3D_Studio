#version 440

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 UV;

out vec2 UV_FS;

uniform mat4 gPersp;
uniform mat4 gWorldToCamera;
uniform mat4 gCameraRotation;
uniform mat4 gLightWVP;
void main()
{
	UV_FS = UV;
			
	vec4 PositionTemp = gPersp*gCameraRotation*vec4(Position.x, Position.y, Position.z, 1.0);
	
	gl_Position = PositionTemp.xyww;
}