#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D tex;

layout(binding = 2) uniform GlobalUniformBufferObject {
	vec3 lightDir;
	vec4 lightColor;
	vec3 eyePos;
} gubo;

void main() {
    vec3 N = normalize(fragNorm);                // surface normal
    vec3 V = normalize(gubo.eyePos - fragPos);    // viewer direction
    vec3 L = normalize(gubo.lightDir);            // light direction

    vec3 albedo = texture(tex, fragUV).rgb;        // main color
    vec3 MD = albedo;
    vec3 MS = albedo + vec3(0.2f);
    vec3 Ambient = albedo * 0.3f;
    
    // Lambert + Blinn
    vec3 DiffSpec = MD * clamp(dot(L,N),0.0f,1.0f) + MS * pow(clamp(dot(N, normalize(L + V)), 0.0f, 1.0f), 160.0f);
    
    outColor = vec4(clamp((0.95) * DiffSpec + Ambient, 0.0f, 0.9f), 1.0f);    // output color
}
