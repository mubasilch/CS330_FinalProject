#pragma once
#include <glm/glm.hpp>

class ShapeMeshes;
class SceneManager;

// Simple interface all scene objects implement.
class Object3D {
public:
    virtual ~Object3D() = default;

    // Load any special meshes this object needs (called once)
    virtual void Prepare(ShapeMeshes* meshes) { (void)meshes; }

    // Draw the object (called every frame)
    virtual void Render(SceneManager& sm, ShapeMeshes& meshes) = 0;

    // Optional world transform helpers (if you want to offset groups later)
    glm::vec3 position{ 0.0f };
    glm::vec3 rotationDeg{ 0.0f };
    glm::vec3 scale{ 1.0f };
};
