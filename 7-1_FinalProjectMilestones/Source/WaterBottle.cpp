#include "WaterBottle.h"
#include "SceneManager.h"
#include "ShapeMeshes.h"
#include <glm/glm.hpp>

void WaterBottle::Prepare(ShapeMeshes* meshes)
{
    meshes->LoadCylinderMesh();
    meshes->LoadTorusMesh(); // for a small ring/handle on the cap
}

void WaterBottle::Render(SceneManager& sm, ShapeMeshes& meshes)
{
    using glm::vec3;

    const float y = 0.02f;

    // Body
    sm.SetTransformations(vec3(0.18f, height * 0.45f, 0.18f), 0, 0, 0, vec3(position.x, y + height * 0.45f, position.z));
    sm.SetShaderColor(0.2f, 0.45f, 0.95f, 1.0f);           // blue tinted plastic
    sm.SetShaderMaterial("plasticBlue");               // shiny a bit
    meshes.DrawCylinderMesh();

    // Neck
    sm.SetTransformations(vec3(0.12f, 0.08f, 0.12f), 0, 0, 0, vec3(position.x, y + height * 0.90f, position.z));
    sm.SetShaderColor(0.85f, 0.9f, 0.95f, 1.0f);
    sm.SetShaderMaterial("plasticWhite");
    meshes.DrawCylinderMesh();

    // Cap
    sm.SetTransformations(vec3(0.13f, 0.05f, 0.13f), 0, 0, 0, vec3(position.x, y + height * 0.99f, position.z));
    sm.SetShaderColor(0.12f, 0.12f, 0.15f, 1.0f);
    sm.SetShaderMaterial("plasticBlack");
    meshes.DrawCylinderMesh();
}
