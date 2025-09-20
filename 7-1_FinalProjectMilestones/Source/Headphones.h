#pragma once
#include "Object3D.h"

class Headphones : public Object3D {
public:
    void Prepare(ShapeMeshes* meshes) override;
    void Render(SceneManager& sm, ShapeMeshes& meshes) override;
};
