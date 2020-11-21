#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
}


#shader fragment
#version 330 core

out vec4 color;

uniform sampler2D screenTexture;

void main()
{
	vec2 uv = gl_FragCoord.xy / vec2(1920/2, 1080/2);
	color = texture(screenTexture, uv);
}
