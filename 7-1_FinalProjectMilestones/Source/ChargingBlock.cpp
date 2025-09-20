#include "ChargingBlock.h"
#include "SceneManager.h"
#include "ShapeMeshes.h"
#include <glm/glm.hpp>

void ChargingBlock::Prepare(ShapeMeshes* meshes)
{
    meshes->LoadCylinderMesh(); // we will “fake” a rounded rectangular block using 3 cylinders
    meshes->LoadTorusMesh();    // for a cable loop if available (falls back if not)
}

void ChargingBlock::Render(SceneManager& sm, ShapeMeshes& meshes)
{
    using glm::vec3;

    const float y = 0.02f + height * 0.5f;

    // Block “body”: three slightly overlapped flat cylinders to approximate a rounded-rectangle brick
    sm.SetTransformations(vec3(width * 0.36f, height * 0.5f, depth * 0.50f), 0, 0, 0, vec3(position.x - width * 0.20f, y, position.z));
    sm.SetShaderColor(0.92f, 0.92f, 0.96f, 1.0f);
    sm.SetShaderMaterial("plasticWhite");
    meshes.DrawCylinderMesh();

    sm.SetTransformations(vec3(width * 0.36f, height * 0.5f, depth * 0.50f), 0, 0, 0, vec3(position.x + width * 0.00f, y, position.z));
    sm.SetShaderMaterial("plasticWhite");
    meshes.DrawCylinderMesh();

    sm.SetTransformations(vec3(width * 0.36f, height * 0.5f, depth * 0.50f), 0, 0, 0, vec3(position.x + width * 0.20f, y, position.z));
    sm.SetShaderMaterial("plasticWhite");
    meshes.DrawCylinderMesh();

    // Cable: a torus laying on the table beside it
    sm.SetTransformations(vec3(0.25f, 0.25f, 0.25f), 90, 0, 0, vec3(position.x + width * 0.6f, 0.04f, position.z));
    sm.SetShaderColor(0.85f, 0.85f, 0.90f, 1.0f);
    sm.SetShaderMaterial("plasticWhite");
    meshes.DrawTorusMesh();
}
