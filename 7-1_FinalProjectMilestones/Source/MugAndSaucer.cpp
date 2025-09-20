#include "MugAndSaucer.h"
#include "ISceneContext.h"
#include "ShapeMeshes.h"

void MugAndSaucer::Render(ISceneContext& ctx)
{
    // === Same constants you used before ===
    const float saucerRadius = 1.20f;
    const float saucerHeight = 0.03f;
    const float saucerY = 0.02f;

    const float mugHeight = 0.90f;
    const float mugRadius = 0.35f;
    const float wall = 0.04f;
    const float footH = 0.03f;
    const float mugBaseY = 0.05f;
    const float mugCenterY = mugBaseY + (mugHeight * 0.5f) + footH;

    // Saucer base
    ctx.SetTransformations({ saucerRadius, saucerHeight, saucerRadius }, 0, 0, 0, { 0.f, saucerY, 0.f });
    ctx.SetShaderColor(0.92f, 0.92f, 0.92f, 1.0f);
    ctx.SetShaderMaterial("ceramic");
    ctx.Meshes()->DrawCylinderMesh();

    // Saucer rim (if torus exists in your ShapeMeshes)
#ifdef HAS_TORUS_MESH
    ctx.SetTransformations({ 1.22f, 1.22f, 1.22f }, 0, 0, 0, { 0.f, saucerY + 0.03f, 0.f });
    ctx.SetShaderColor(0.88f, 0.88f, 0.88f, 1.0f);
    ctx.SetShaderMaterial("ceramic");
    ctx.Meshes()->DrawTorusMesh();
#endif

    // Mug foot
    ctx.SetTransformations({ mugRadius * 0.90f, footH, mugRadius * 0.90f }, 0, 0, 0,
        { 0.f, mugBaseY + footH * 0.5f, 0.f });
    ctx.SetShaderColor(0.75f, 0.75f, 0.75f, 1.0f);
    ctx.SetShaderMaterial("ceramic");
    ctx.Meshes()->DrawCylinderMesh();

    // Mug outer body
    ctx.SetTransformations({ mugRadius, mugHeight * 0.5f, mugRadius }, 0, 0, 0,
        { 0.f, mugCenterY, 0.f });
    if (ctx.HasTexture("ceramicTex")) { ctx.SetShaderTexture("ceramicTex"); ctx.SetTextureUVScale(1.0f, 1.0f); }
    else { ctx.SetShaderColor(0.95f, 0.95f, 0.98f, 1.0f); }
    ctx.SetShaderMaterial("ceramic");
    ctx.Meshes()->DrawCylinderMesh();

    // Mug inner liner
    {
        float innerH = mugHeight * 0.92f;
        float innerR = mugRadius - wall;
        ctx.SetTransformations({ innerR, innerH * 0.5f, innerR }, 0, 0, 0,
            { 0.f, mugBaseY + footH + innerH * 0.5f + 0.01f, 0.f });
        ctx.SetShaderColor(0.96f, 0.96f, 0.99f, 1.0f);
        ctx.SetShaderMaterial("ceramic");
        ctx.Meshes()->DrawCylinderMesh();
    }

#ifdef HAS_TORUS_MESH
    // Lip
    ctx.SetTransformations({ mugRadius + 0.005f, mugRadius + 0.005f, mugRadius + 0.005f }, 0, 0, 0,
        { 0.f, mugBaseY + footH + mugHeight + 0.005f, 0.f });
    ctx.SetShaderColor(0.90f, 0.90f, 0.93f, 1.0f);
    ctx.SetShaderMaterial("ceramic");
    ctx.Meshes()->DrawTorusMesh();

    // Handle
    ctx.SetTransformations({ 0.18f, 0.28f, 0.18f }, 0, 0, 90.f,
        { mugRadius + 0.02f, mugBaseY + footH + mugHeight * 0.55f, 0.f });
    ctx.SetShaderColor(0.95f, 0.95f, 0.98f, 1.0f);
    ctx.SetShaderMaterial("ceramic");
    ctx.Meshes()->DrawTorusMesh();
#endif
}
