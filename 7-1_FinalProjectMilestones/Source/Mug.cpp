#include "Mug.h"
#include "SceneManager.h"
#include "ShapeMeshes.h"
#include <glm/glm.hpp>

#ifndef HAS_TORUS_MESH
#define HAS_TORUS_MESH 1
#endif

void Mug::Prepare(ShapeMeshes* meshes)
{
    meshes->LoadCylinderMesh();
#if HAS_TORUS_MESH
    meshes->LoadTorusMesh();
#else
    useTorusHandle = false;
#endif
}

void Mug::Render(SceneManager& sm, ShapeMeshes& meshes)
{
    using glm::vec3;

    const float wall = 0.04f;
    const float footH = 0.03f;

    // put on table (~ same “saucer” Y your scene uses)
    const float baseY = 0.07f;
    const float centerY = baseY + (mugHeight * 0.5f) + footH;

    // Foot
    sm.SetTransformations(vec3(mugRadius * 0.90f, footH, mugRadius * 0.90f), 0, 0, 0,
        vec3(position.x, baseY + footH * 0.5f, position.z));
    sm.SetShaderColor(0.75f, 0.75f, 0.75f, 1.0f);
    sm.SetShaderMaterial("ceramic");
    meshes.DrawCylinderMesh();

    // Outer body
    sm.SetTransformations(vec3(mugRadius, mugHeight * 0.5f, mugRadius), 0, 0, 0,
        vec3(position.x, centerY, position.z));
    sm.SetShaderColor(0.95f, 0.95f, 0.98f, 1.0f); // or use ceramicTex if you like
    sm.SetShaderMaterial("ceramic");
    meshes.DrawCylinderMesh();

    // Inner liner (fake cavity)
    const float innerH = mugHeight * 0.92f, innerR = mugRadius - wall;
    sm.SetTransformations(vec3(innerR, innerH * 0.5f, innerR), 0, 0, 0,
        vec3(position.x, baseY + footH + innerH * 0.5f + 0.01f, position.z));
    sm.SetShaderColor(0.96f, 0.96f, 0.99f, 1.0f);
    sm.SetShaderMaterial("ceramic");
    meshes.DrawCylinderMesh();

    // Lip (two thin rings as thin cylinders)
    const float lipY = baseY + footH + mugHeight + 0.002f;
    sm.SetTransformations(vec3(mugRadius + 0.01f, 0.01f, mugRadius + 0.01f), 0, 0, 0,
        vec3(position.x, lipY, position.z));
    sm.SetShaderColor(0.90f, 0.90f, 0.93f, 1.0f);
    sm.SetShaderMaterial("ceramic");
    meshes.DrawCylinderMesh();

    sm.SetTransformations(vec3(mugRadius - 0.005f, 0.008f, mugRadius - 0.005f), 0, 0, 0,
        vec3(position.x, lipY + 0.002f, position.z));
    sm.SetShaderColor(0.95f, 0.95f, 0.98f, 1.0f);
    sm.SetShaderMaterial("ceramic");
    meshes.DrawCylinderMesh();

    // Handle (torus if available, otherwise 3 cylinders shaped like a “C”)
#if HAS_TORUS_MESH
    {
        const float hy = baseY + footH + mugHeight * 0.55f;
        sm.SetTransformations(vec3(0.18f, 0.28f, 0.18f), 0, 0, 90.0f,
            vec3(position.x + mugRadius + 0.02f, hy, position.z));
        sm.SetShaderColor(0.95f, 0.95f, 0.98f, 1.0f);
        sm.SetShaderMaterial("ceramic");
        meshes.DrawTorusMesh();
    }
#else
    // top
    sm.SetTransformations(vec3(0.04f, 0.10f, 0.04f), 0, 0, 90.0f,
        vec3(position.x + mugRadius + 0.02f,
            baseY + footH + mugHeight * 0.70f, position.z));
    sm.SetShaderColor(0.95f, 0.95f, 0.98f, 1.0f);
    sm.SetShaderMaterial("ceramic");
    meshes.DrawCylinderMesh();
    // middle
    sm.SetTransformations(vec3(0.04f, 0.12f, 0.04f), 90, 0, 0,
        vec3(position.x + mugRadius + 0.10f,
            baseY + footH + mugHeight * 0.58f, position.z));
    sm.SetShaderColor(0.95f, 0.95f, 0.98f, 1.0f);
    sm.SetShaderMaterial("ceramic");
    meshes.DrawCylinderMesh();
    // bottom
    sm.SetTransformations(vec3(0.04f, 0.10f, 0.04f), 0, 0, 90.0f,
        vec3(position.x + mugRadius + 0.02f,
            baseY + footH + mugHeight * 0.46f, position.z));
    sm.SetShaderColor(0.95f, 0.95f, 0.98f, 1.0f);
    sm.SetShaderMaterial("ceramic");
    meshes.DrawCylinderMesh();
#endif
}
