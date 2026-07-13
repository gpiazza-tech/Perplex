#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_Center;
layout (location = 2) in vec2 a_Bounds;
layout (location = 3) in vec4 a_Color;
layout (location = 4) in float a_Emission;

uniform mat4 u_ViewProj;
uniform mat4 u_Transform;

out vec2 v_Center;
out vec2 v_Bounds;
out vec4 v_Color;
out float v_Emission;
out vec2 v_WorldCoord;

void main()
{
	v_Center = a_Center;
	v_Bounds = a_Bounds;
	v_Color = a_Color;
	v_Emission = a_Emission;
	v_WorldCoord = a_Position.xy;
	gl_Position = u_ViewProj * u_Transform * vec4(a_Position, 1.0);
}