#include "TablePlane.h"
#include "ISceneContext.h"
#include "ShapeMeshes.h"

void TablePlane::Render(ISceneContext& ctx)
{
    ctx.SetTransformations(m_scale, 0.f, 0.f, 0.f, m_pos);

    if (ctx.HasTexture(m_woodTag))          ctx.SetShaderTexture(m_woodTag);
    else if (ctx.HasTexture(m_woodTagFallback)) ctx.SetShaderTexture(m_woodTagFallback);
    else                                     ctx.SetShaderColor(0.55f, 0.40f, 0.25f, 1.0f); // fallback

    ctx.SetTextureUVScale(m_uv.x, m_uv.y);
    ctx.SetShaderMaterial(m_materialTag);

    ctx.Meshes()->DrawPlaneMesh();
}
