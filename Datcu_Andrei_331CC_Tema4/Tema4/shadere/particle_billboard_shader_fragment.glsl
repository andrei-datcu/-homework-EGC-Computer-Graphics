#version 330

uniform sampler2D texUnit;

in vec2 texCoord;
in vec3 fColor;
out vec4 out_color;

void main()
{
    out_color = texture2D(texUnit, texCoord);
	if (out_color.r > 0.9 && out_color.g > 0.9 && out_color.b > 0.9)
        discard;
	else
		out_color = vec4(fColor, 1);
} 