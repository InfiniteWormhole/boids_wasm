#version 300 es
precision lowp float;
out vec4 FragColor;
in vec4 coord;
in vec4 col;

uniform float size;
uniform float aspect;

float aastep(float threshold, float value) {
  float afwidth = 0.7 * length(vec2(dFdx(value), dFdy(value)));
  return smoothstep(threshold-afwidth, threshold+afwidth, value);
}

void main()
{
	if(col.a == 0.0)
	{
		discard;
	}
    // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	// FragColor = step(length(coord), vec4(0.0));
	// FragColor = gl_FragCoord / vec4(1000.0, 1000.0, 1000.0, 1.0);
	// FragColor /= coord / vec4(1000.0, 1000.0, 1000.0, 1.0);
	FragColor.rgb = col.rgb;
	// FragColor.a = 1.0;
	FragColor.a = aastep(-(size) * 2.0, -1.0 * length(coord.xyz * vec3(1.0, aspect, 1.0))*100.0);
	//FragColor.a += aastep(-5.0, -1.0 * length(coord.xyz * vec3(1.0, aspect, 1.0))*100.0)/6.0;
	// FragColor.a += 0.2;
	if(FragColor.a == 0.0)
	{
		discard;
	}
	else
	{
		// FragColor.a /= 1.428;
		FragColor.a /= 2.0;
	}
	// FragColor = vec4(normalize(gl_FragCoord).rgb, 1.0);
	// FragColor = gl_FragCoord;
}