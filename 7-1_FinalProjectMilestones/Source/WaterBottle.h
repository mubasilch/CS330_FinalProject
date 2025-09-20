#pragma once
#include "Object3D.h"

class WaterBottle : public Object3D {
public:
    float height = 1.1f;
    void Prepare(ShapeMeshes* meshes) override;
    void Render(SceneManager& sm, ShapeMeshes& meshes) override;
};
