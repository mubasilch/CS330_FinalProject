#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 UVscale = vec2(1.0, 1.0);

out VS_OUT {
    vec3 FragPos;   // world-space
    vec3 Normal;    // world-space
    vec2 UV;
} vs;

void main()
{
    vec4 worldPos   = model * vec4(aPos, 1.0);
    vs.FragPos      = worldPos.xyz;

    // Normal matrix to keep normals correct for non-uniform scale
    mat3 normalMat  = transpose(inverse(mat3(model)));
    vs.Normal       = normalize(normalMat * aNormal);

    vs.UV           = aTex * UVscale;

    gl_Position = projection * view * worldPos;
}
