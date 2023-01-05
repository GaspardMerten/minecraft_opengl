#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 P;
uniform mat4 V;

void main()
{
    TexCoords = aPos;
    mat4 V_no_rot = mat4(mat3(V)) ;
    vec4 pos = P * V_no_rot * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}  