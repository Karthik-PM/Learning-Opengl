#version 330 core

layout (location = 0) in vec3 aPos1;

void main(){
   gl_Position = vec4(aPos1.x, aPos1.y, aPos1.z, 1.0);
}