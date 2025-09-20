#pragma once
#include "Object3D.h"

class Monitor : public Object3D {
public:
    // size in “meters” (scene units)
    float width = 2.0f;
    float height = 1.2f;
    float depth = 0.06f;

    void Prepare(ShapeMeshes* meshes) override;
    void Render(SceneManager& sm, ShapeMeshes& meshes) override;
};
