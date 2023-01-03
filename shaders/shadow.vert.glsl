#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
    vec4 frag_coord = M * vec4(position, 1.0);
    vec4 tmp = P*V*frag_coord;
    gl_Position = tmp;
}