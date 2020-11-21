#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main()
{
	gl_Position = position;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform float timePassed;

void main()
{
   float offsetR = sin((timePassed + 100.) * 10. + (gl_FragCoord.x + gl_FragCoord.y) / 10.) + 1.0 / 2.0;
   float offsetG = sin(timePassed * 10. + (gl_FragCoord.x + gl_FragCoord.y) / 10.) + 1.0 / 2.0;
   float offsetB = sin((timePassed - 100.) * 10. + (gl_FragCoord.x + gl_FragCoord.y) / 10.) + 1.0 / 2.0;
   color = vec4(offsetR, offsetG, offsetB, 1.0);
}
