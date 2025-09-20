#pragma once
#include "Object3D.h"

class Laptop : public Object3D {
public:
    float baseW = 1.8f, baseD = 1.2f, baseH = 0.05f;
    float scrW = 1.7f, scrH = 1.0f, scrT = 0.04f;
    float hingeLift = 0.07f;
    float openAngleDeg = 105.0f; // hinge angle

    void Prepare(ShapeMeshes* meshes) override;
    void Render(SceneManager& sm, ShapeMeshes& meshes) override;
};
