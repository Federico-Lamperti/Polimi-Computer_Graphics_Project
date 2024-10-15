#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec2 inUV;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 mvpMat;
    mat4 nMat;
    mat4 mMat;
} ubo;

layout(location = 0) out vec3 fragPos;

void main() {
    fragPos = inPos;
    gl_Position = (ubo.mvpMat * vec4(inPos, 1.0)).xyww;
}
