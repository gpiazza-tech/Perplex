#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in float a_Slope;
layout (location = 2) in float a_YIntercept;
layout (location = 3) in vec4 a_Color;
layout (location = 4) in float a_Emission;

uniform mat4 u_ViewProj;
uniform mat4 u_Transform;

out float v_Slope;
out float v_YIntercept;
out vec4 v_Color;
out float v_Emission;
out vec2 v_WorldCoord;

void main()
{
	v_Slope = a_Slope;
	v_YIntercept = a_YIntercept;
	v_Color = a_Color;
	v_Emission = a_Emission;
	v_WorldCoord = a_Position.xy;
	gl_Position = u_ViewProj * u_Transform * vec4(a_Position, 1.0);
}