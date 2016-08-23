#version 330 core
layout(location = 0) in vec2 vertexPos;

void main() {
    gl_Position.x = vertexPos.x*2 - 1;
    gl_Position.y = vertexPos.y*2 - 1;
    gl_Position.z = 1.0;
    gl_Position.w =  1.0;
}