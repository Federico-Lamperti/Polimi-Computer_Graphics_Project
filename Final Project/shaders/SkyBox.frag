#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform samplerCube tex;

void main() {
    outColor = texture(tex, fragPos);
}
