#version 450

layout(location = 0) in vec3 fragColor;

layout(location = 0) out uvec4 outColor;

void main() {
    outColor = uvec4(fragColor * 255.0, 255);
}
