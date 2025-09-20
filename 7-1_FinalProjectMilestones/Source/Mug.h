#pragma once
#include "Object3D.h"

class Mug : public Object3D {
public:
    bool useTorusHandle = true; // will fallback in cpp if torus not present
    float mugHeight = 0.90f;
    float mugRadius = 0.35f;

    void Prepare(ShapeMeshes* meshes) override;
    void Render(SceneManager& sm, ShapeMeshes& meshes) override;
};
