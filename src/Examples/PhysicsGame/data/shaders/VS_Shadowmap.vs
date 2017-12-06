#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;

uniform mat4 gRotation;
uniform mat4 gTranslation;
uniform mat4 gWorldToLight;
uniform mat4 gPersp;
uniform mat4 gScale;


void main()
{
    gl_Position = gPersp*gWorldToLight*gTranslation*gRotation * gScale* vec4(Position, 1.0);
} 