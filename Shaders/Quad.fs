#version 430 core

out vec4 FragColor;

uniform sampler2D image;

in vec2 uv;

void main() {
    vec3 result = texture(image,uv).rgb;
    FragColor = vec4(result,1.0);
}