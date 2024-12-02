#version 330 core
out vec4 FragColor;
//in vec3 vertexColor;
in vec2 TexCoord;

uniform sampler2D mytexture;

void main() {
    //FragColor = texture(texture1, TexCoord);
    FragColor =  texture(mytexture, TexCoord) * vec4(1.0f, 0.5f, 0.2f, 1.0f);
//    FragColor =  vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
