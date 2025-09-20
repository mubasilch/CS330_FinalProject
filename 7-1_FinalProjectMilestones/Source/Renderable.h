#pragma once
#include "ISceneContext.h"

// Base interface for scene objects
class Renderable {
public:
    virtual ~Renderable() = default;
    // Called once after meshes/materials/textures are ready
    virtual void Prepare(ISceneContext& ctx) = 0;
    // Called every frame to draw
    virtual void Render(ISceneContext& ctx) = 0;
};
