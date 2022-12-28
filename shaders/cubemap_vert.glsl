#version 330 core
in vec3 position;
in vec2 tex_coord;
in vec3 normal;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
out vec3 texCoord_v;

void main(){
    texCoord_v = position;
    mat4 V_no_rot = mat4(mat3(V)) ;
    vec4 pos = P * V_no_rot * vec4(position, 1.0);
    gl_Position = pos.xyww;
}