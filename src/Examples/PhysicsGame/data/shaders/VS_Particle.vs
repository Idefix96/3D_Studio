#version 440 

layout (location = 0) in vec3 Position[];
layout (location = 1) in vec3 Velocity[];
layout (location = 2) in float Age[];

out vec3 Position0;
out vec3 Velocity0;
out float Age0;

uniform float gDeltaTimeMillis;

void main()
{
    Position0 = Position[0];
    Velocity0 = Velocity[0];
    Age0 = Age[0];
	gl_Position = vec4(Position0,1.0f);
}