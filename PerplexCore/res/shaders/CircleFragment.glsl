#version 330 core

layout (location = 0) out vec4 o_Color;

uniform float u_PixelsPerUnit;

in vec2 v_Center;
in float v_Radius;
in float v_Thickness;
in vec4 v_Color;
in float v_Emission;

in vec2 v_WorldCoord;

vec2 make_pixel_perfect(in vec2 pos)
{
	float x = float(int(pos.x * u_PixelsPerUnit)) / u_PixelsPerUnit;
    float y = float(int(pos.y * u_PixelsPerUnit)) / u_PixelsPerUnit;

	return vec2(x, y);
}

void main()
{
	vec2 diff = v_WorldCoord - v_Center;
	vec2 pixelDiff = make_pixel_perfect(diff);
    
	float thickness = min(v_Thickness, v_Radius);

	float sqrDist = pixelDiff.x * pixelDiff.x + pixelDiff.y * pixelDiff.y;
	float sqrRad = v_Radius * v_Radius;
	float innerSqrRad = (v_Radius - thickness) * (v_Radius - thickness);
    
	if (sqrDist >= sqrRad || (v_Radius != thickness && sqrDist <= innerSqrRad))
		discard;
    
	o_Color = v_Color + v_Emission;
}