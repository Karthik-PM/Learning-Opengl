#version 330 core
out vec4 FragColor;
uniform vec4 cpuInput;
void main()
{
    //FragColor = vec4(1.0f, 0.1f, 0.2f, 1.0f);
    FragColor = cpuInput;
} 