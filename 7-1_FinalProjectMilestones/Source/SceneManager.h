///////////////////////////////////////////////////////////////////////////////
// SceneManager.h
// ============
// Manage the loading and rendering of 3D scenes
//
//  AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
//  Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ShaderManager.h"
#include "ShapeMeshes.h"

#include <string>
#include <vector>
#include <memory>

class Object3D;

class SceneManager
{
public:
    SceneManager(ShaderManager* pShaderManager);
    ~SceneManager();

    struct TEXTURE_INFO {
        std::string tag; uint32_t ID;
    };

    struct OBJECT_MATERIAL {
        float ambientStrength;
        glm::vec3 ambientColor;
        glm::vec3 diffuseColor;
        glm::vec3 specularColor;
        float shininess;
        std::string tag;
    };

private:
    ShaderManager* m_pShaderManager;
    ShapeMeshes* m_basicMeshes;

    int m_loadedTextures;
    TEXTURE_INFO m_textureIDs[16];
    std::vector<OBJECT_MATERIAL> m_objectMaterials;

    // textures
    bool CreateGLTexture(const char* filename, std::string tag);
    void BindGLTextures();
    void DestroyGLTextures();
    int  FindTextureID(std::string tag);
    int  FindTextureSlot(std::string tag);
    bool FindMaterial(std::string tag, OBJECT_MATERIAL& material);


public:
    // called from Main
    void PrepareScene();
    void RenderScene();

    // allow Object classes to use these helpers
    friend class Object3D; // lets them call SetTransformations etc. through SceneManager ref

    // shader setters
    void SetTransformations(glm::vec3 scaleXYZ, float Xdeg, float Ydeg, float Zdeg, glm::vec3 posXYZ);
    void SetShaderColor(float r, float g, float b, float a);
    void SetShaderTexture(std::string textureTag);
    void SetTextureUVScale(float u, float v);
    void SetShaderMaterial(std::string materialTag);

private:
    // === NEW: object ownership ===
    std::vector<std::unique_ptr<Object3D>> m_objects;

    // helper to add an object
    template <typename T, typename... Args>
    T* AddObject(Args&&... args) {
        auto obj = std::make_unique<T>(std::forward<Args>(args)...);
        T* raw = obj.get();
        m_objects.emplace_back(std::move(obj));
        return raw;
    }
};
