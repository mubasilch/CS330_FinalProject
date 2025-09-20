#pragma once
#include "Object3D.h"

class ChargingBlock : public Object3D {
public:
    float width = 0.40f, depth = 0.25f, height = 0.25f;
    void Prepare(ShapeMeshes* meshes) override;
    void Render(SceneManager& sm, ShapeMeshes& meshes) override;
};
