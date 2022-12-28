#version 330 core
out vec4 FragColor;
precision mediump float;
in vec4 v_col;
in vec2 v_t;
void main() {
    FragColor = v_col;
}