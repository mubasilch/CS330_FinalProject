#include "Monitor.h"
#include "SceneManager.h"
#include "ShapeMeshes.h"
#include <glm/glm.hpp>

void Monitor::Prepare(ShapeMeshes* meshes)
{
    meshes->LoadPlaneMesh();     // for screen
    meshes->LoadCylinderMesh();  // for stand + base
}

void Monitor::Render(SceneManager& sm, ShapeMeshes& meshes)
{
    using glm::vec3;

    // Screen — a very thin “slab” using two planes (front/back)
    const float y = 1.25f;              // lift off table
    const float t = depth;              // thin offset so it has perceived thickness

    // Front
    sm.SetTransformations(vec3(width, 1.0f, height), 90.0f, 0, 0, vec3(position.x, y, position.z));
    sm.SetShaderTexture("screen");  // <- add a 1024+ jpg/png named "screen" to textures to get a nice monitor image
    sm.SetTextureUVScale(1.0f, 1.0f);
    sm.SetShaderMaterial("plasticBlack");
    meshes.DrawPlaneMesh();

    // Back (dark)
    sm.SetTransformations(vec3(width, 1.0f, height), 90.0f, 0, 0, vec3(position.x, y - t, position.z));
    sm.SetShaderColor(0.05f, 0.05f, 0.07f, 1.0f);
    sm.SetShaderMaterial("plasticBlack");
    meshes.DrawPlaneMesh();

    // Stand pole
    sm.SetTransformations(vec3(0.05f, 0.30f, 0.05f), 0, 0, 0, vec3(position.x, y - 0.35f, position.z - 0.05f));
    sm.SetShaderColor(0.1f, 0.1f, 0.12f, 1.0f);
    sm.SetShaderMaterial("metal");
    meshes.DrawCylinderMesh();

    // Base disk
    sm.SetTransformations(vec3(0.55f, 0.03f, 0.35f), 0, 0, 0, vec3(position.x, 0.02f, position.z - 0.05f));
    sm.SetShaderColor(0.08f, 0.08f, 0.10f, 1.0f);
    sm.SetShaderMaterial("metal");
    meshes.DrawCylinderMesh();
}
