#version 330 core
in vec3 position;
in vec2 tex_coord;
in vec3 normal;
out vec4 v_col;
out vec2 v_t;
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
void main(){
    gl_Position = P*M*V*vec4(position, 1);
    v_col = vec4(normal*0.5 + 0.5, 1.0);
    v_t = tex_coord;
}