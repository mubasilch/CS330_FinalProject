#pragma once
#include <glm/glm.hpp>
#include <string>

class ShapeMeshes;

// A minimal API the scene objects can use to render themselves.
// SceneManager will implement this.
class ISceneContext {
public:
    virtual ~ISceneContext() = default;

    virtual void SetTransformations(glm::vec3 scaleXYZ,
        float Xdeg, float Ydeg, float Zdeg,
        glm::vec3 pos) = 0;

    virtual void SetShaderColor(float r, float g, float b, float a) = 0;
    virtual void SetShaderTexture(std::string tag) = 0;
    virtual void SetTextureUVScale(float u, float v) = 0;
    virtual void SetShaderMaterial(std::string tag) = 0;

    // Mesh access so objects can issue draws
    virtual ShapeMeshes* Meshes() = 0;

    // Utility: check if a texture tag exists (for graceful fallback)
    virtual bool HasTexture(std::string tag) const = 0;
};
