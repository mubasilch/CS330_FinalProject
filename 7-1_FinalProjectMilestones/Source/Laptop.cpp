#include "Laptop.h"
#include "SceneManager.h"
#include "ShapeMeshes.h"
#include <glm/glm.hpp>

void Laptop::Prepare(ShapeMeshes* meshes)
{
    meshes->LoadPlaneMesh();    // for top surfaces
    meshes->LoadCylinderMesh(); // for hinge disks / give thickness
}

void Laptop::Render(SceneManager& sm, ShapeMeshes& meshes)
{
    using glm::vec3;

    const float tableY = 0.02f;

    // Base as a very flat cylinder “tablet”
    sm.SetTransformations(vec3(baseW * 0.55f, baseH, baseD * 0.55f), 0, 0, 0, vec3(position.x, tableY + baseH * 0.5f, position.z));
    sm.SetShaderTexture("aluminum");   // add aluminum.jpg if you have it; otherwise it falls back to color used previously
    sm.SetTextureUVScale(1, 1);
    sm.SetShaderMaterial("metal");
    meshes.DrawCylinderMesh();

    // Keyboard “plane” (texture optional)
    sm.SetTransformations(vec3(baseW, 1.0f, baseD), 90, 0, 0, vec3(position.x, tableY + baseH + 0.001f, position.z));
    sm.SetShaderTexture("keyboard"); // optional 1024+ texture; otherwise comment and color
    sm.SetTextureUVScale(1, 1);
    sm.SetShaderMaterial("metal");
    meshes.DrawPlaneMesh();

    // Screen: hinged rectangle made from a plane; rotate around back edge
    // Move pivot to back hinge -> translate to hinge, rotate, translate back
    const float hingeY = tableY + baseH + hingeLift;
    const float cz = position.z - baseD * 0.48f; // hinge near back
    // emulate rotation by composing transforms: R * T
    // Here we approximate by drawing the screen above the hinge with rotation around X.
    sm.SetTransformations(vec3(scrW, 1.0f, scrH), 90 - openAngleDeg, 0, 0, vec3(position.x, hingeY + scrH * 0.5f, cz));
    sm.SetShaderTexture("screen"); // reuse same monitor texture if present
    sm.SetTextureUVScale(1, 1);
    sm.SetShaderMaterial("plasticBlack");
    meshes.DrawPlaneMesh();

    // Screen back (dark)
    sm.SetTransformations(vec3(scrW, 1.0f, scrH), 90 - openAngleDeg, 0, 0, vec3(position.x, hingeY + scrH * 0.5f - 0.01f, cz));
    sm.SetShaderColor(0.05f, 0.05f, 0.07f, 1.0f);
    sm.SetShaderMaterial("plasticBlack");
    meshes.DrawPlaneMesh();
}
