#version 300 es
precision lowp float;
layout (location = 0) in lowp vec3 aPos;
layout (location = 1) in lowp vec4 pPos;
layout (location = 2) in lowp vec4 pCol;
out vec4 coord;
out vec4 col;
void main()
{
    gl_Position = vec4(aPos.x + pPos.x, aPos.y + pPos.y, aPos.z, 1.0);
	// vec3 pos = normalize(aPos);
	// coord = vec4(vec3(pos.x + 0.25, pos.y, pos.z), 1.0);
	coord = vec4(aPos, 1.0);
	col = vec4(pCol.rgba);
}