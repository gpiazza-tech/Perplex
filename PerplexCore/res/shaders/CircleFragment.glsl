#version 330 core

layout (location = 0) out vec4 o_Color;

uniform float u_PixelsPerUnit;

in vec2 v_Center;
in float v_Radius;
in float v_Thickness;
in vec4 v_Color;
in float v_Emission;

in vec2 v_WorldCoord;

bool vertical_circle_intercepts(vec2 center, float radius, float x_line, out vec2 intercepts) 
{
    // Calculate the horizontal distance from the line to the circle center
    float dx = x_line - center.x;
    
    // Check if the vertical line is within the circle's horizontal bounds
    if (abs(dx) > radius) {
        return false; // No intersection
    }
    
    // Calculate the vertical offset from the circle center to the intercepts
    float dy = sqrt(radius * radius - dx * dx);
    
    // Return both the lower and upper y-intercepts
    intercepts.x = center.y - dy; // Lower intercept
    intercepts.y = center.y + dy; // Upper intercept
    
    return true;
}

bool horizontal_circle_intercepts(vec2 center, float radius, float y_line, out vec2 intercepts) 
{
    // Calculate the horizontal distance from the line to the circle center
    float dy = y_line - center.y;
    
    // Check if the vertical line is within the circle's horizontal bounds
    if (abs(dy) > radius) {
        return false; // No intersection
    }
    
    // Calculate the vertical offset from the circle center to the intercepts
    float dx = sqrt(radius * radius - dy * dy);
    
    // Return both the lower and upper y-intercepts
    intercepts.x = center.x - dx; // Lower intercept
    intercepts.y = center.x + dx; // Upper intercept
    
    return true;
}

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
    
	float sqrDist = pixelDiff.x * pixelDiff.x + pixelDiff.y * pixelDiff.y;
	float sqrRad = v_Radius * v_Radius;
	float innerSqrRad = (v_Radius - v_Thickness) * (v_Radius - v_Thickness);
    
	if (sqrDist >= sqrRad || (sqrDist <= innerSqrRad && innerSqrRad != 0.0f))
		discard;
    
	o_Color = v_Color + v_Emission;
}