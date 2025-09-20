#include "Headphones.h"
#include "SceneManager.h"
#include "ShapeMeshes.h"
#include <glm/glm.hpp>

void Headphones::Prepare(ShapeMeshes* meshes)
{
    meshes->LoadTorusMesh();    // headband
    meshes->LoadCylinderMesh(); // earcups
}

void Headphones::Render(SceneManager& sm, ShapeMeshes& meshes)
{
    using glm::vec3;

    const float y = 0.06f;

    // Headband (torus standing up)
    sm.SetTransformations(vec3(0.35f, 0.45f, 0.35f), 0, 0, 90.0f,
        vec3(position.x, y + 0.25f, position.z));
    sm.SetShaderColor(0.15f, 0.15f, 0.18f, 1.0f);
    sm.SetShaderMaterial("plasticBlack");
    meshes.DrawTorusMesh();

    // Left earcup
    sm.SetTransformations(vec3(0.10f, 0.12f, 0.10f), 0, 0, 0,
        vec3(position.x - 0.28f, y + 0.12f, position.z));
    sm.SetShaderColor(0.20f, 0.20f, 0.22f, 1.0f);
    sm.SetShaderMaterial("plasticBlack");
    meshes.DrawCylinderMesh();

    // Right earcup
    sm.SetTransformations(vec3(0.10f, 0.12f, 0.10f), 0, 0, 0,
        vec3(position.x + 0.28f, y + 0.12f, position.z));
    sm.SetShaderMaterial("plasticBlack");
    meshes.DrawCylinderMesh();
}
