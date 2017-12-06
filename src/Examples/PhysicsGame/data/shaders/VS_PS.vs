#version 440

layout (location = 0) in vec3 Vertex;
layout (location = 1) in vec4 Position;
layout (location = 2) in vec4 Color;

out vec4 Color_FS;
out vec2 UV_FS;

uniform mat4 gPersp;
uniform mat4 gWorldToCamera;
uniform vec3 CameraRight;
uniform vec3 CameraUp;
uniform vec3 CameraLook;

void main()
{
	Color_FS = Color;
	UV_FS = vec2(4.0*Vertex.x+0.5, 4.0*Vertex.y+0.5);
	gl_Position = gPersp*(gWorldToCamera*(Position) + Vertex.x*vec4(1,0,0,0.0)  + Vertex.y*vec4(0,1,0,0.0));
}