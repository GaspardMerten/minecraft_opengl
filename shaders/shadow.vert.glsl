#version 330 core
in vec3 position;
in vec3 normal;


uniform mat4 lightSpaceMatrix;
uniform mat4 M;
out vec3 v_normal;

void main(){
    v_normal = normal;
    gl_Position = lightSpaceMatrix * M * vec4(position, 1.0);
}