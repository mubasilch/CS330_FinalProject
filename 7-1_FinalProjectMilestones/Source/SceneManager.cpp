///////////////////////////////////////////////////////////////////////////////
// SceneManager.cpp
// ============
// manage the loading and rendering of 3D scenes
// Mubasil Choudhry
// AUTHOR: Brian Battersby - SNHU Instructor / Computer Science
// Created for CS-330-Computational Graphics and Visualization, Nov. 1st, 2023
///////////////////////////////////////////////////////////////////////////////
#include "SceneManager.h"
#include "Object3D.h"
#include "Mug.h"
#include "Monitor.h"
#include "Laptop.h"
#include "ChargingBlock.h"
#include "Headphones.h"
#include "WaterBottle.h"

#define HAS_TORUS_MESH 1
#include <GL/glew.h>
#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif
#include <algorithm>
#include <iostream>
#include <glm/gtx/transform.hpp>

namespace {
    const char* g_ModelName = "model";
    const char* g_ColorValueName = "objectColor";
    const char* g_TextureValueName = "objectTexture";
    const char* g_UseTextureName = "bUseTexture";
}

SceneManager::SceneManager(ShaderManager* pShaderManager)
    : m_pShaderManager(pShaderManager), m_basicMeshes(new ShapeMeshes()), m_loadedTextures(0) {
}

SceneManager::~SceneManager()
{
    m_pShaderManager = nullptr;
    delete m_basicMeshes;  m_basicMeshes = nullptr;
    DestroyGLTextures();
}

// ---------- texture helpers ----------
bool SceneManager::CreateGLTexture(const char* filename, std::string tag)
{
    int w = 0, h = 0, c = 0; GLuint tex = 0;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* img = stbi_load(filename, &w, &h, &c, 0);
    if (!img) { std::cout << "WARN: could not load " << filename << "\n"; return false; }

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (c == 3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    else if (c == 4) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    else { stbi_image_free(img); glBindTexture(GL_TEXTURE_2D, 0); return false; }

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(img); glBindTexture(GL_TEXTURE_2D, 0);

    m_textureIDs[m_loadedTextures].ID = tex;
    m_textureIDs[m_loadedTextures].tag = tag;
    ++m_loadedTextures; return true;
}
void SceneManager::BindGLTextures() {
    for (int i = 0;i < std::min(m_loadedTextures, 16);++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textureIDs[i].ID);
    }
}
void SceneManager::DestroyGLTextures() {
    for (int i = 0;i < m_loadedTextures;++i)
        if (m_textureIDs[i].ID) { glDeleteTextures(1, &m_textureIDs[i].ID); m_textureIDs[i].ID = 0; }
    m_loadedTextures = 0;
}
int  SceneManager::FindTextureID(std::string tag) {
    for (int i = 0;i < m_loadedTextures;++i) if (m_textureIDs[i].tag == tag) return m_textureIDs[i].ID;
    return -1;
}
int  SceneManager::FindTextureSlot(std::string tag) {
    for (int i = 0;i < m_loadedTextures;++i) if (m_textureIDs[i].tag == tag) return i;
    return -1;
}
bool SceneManager::FindMaterial(std::string tag, OBJECT_MATERIAL& m) {
    for (auto& it : m_objectMaterials) if (it.tag == tag) { m = it; return true; } return false;
}

// ---------- shader setters ----------
void SceneManager::SetTransformations(glm::vec3 s, float rx, float ry, float rz, glm::vec3 p)
{
    glm::mat4 model = glm::translate(p)
        * glm::rotate(glm::radians(rx), glm::vec3(1, 0, 0))
        * glm::rotate(glm::radians(ry), glm::vec3(0, 1, 0))
        * glm::rotate(glm::radians(rz), glm::vec3(0, 0, 1))
        * glm::scale(s);
    if (m_pShaderManager) m_pShaderManager->setMat4Value(g_ModelName, model);
}
void SceneManager::SetShaderColor(float r, float g, float b, float a) {
    if (!m_pShaderManager) return;
    m_pShaderManager->setIntValue(g_UseTextureName, false);
    m_pShaderManager->setVec4Value(g_ColorValueName, glm::vec4(r, g, b, a));
}
void SceneManager::SetShaderTexture(std::string tag) {
    if (!m_pShaderManager) return;
    int slot = FindTextureSlot(tag);
    if (slot < 0) { m_pShaderManager->setIntValue(g_UseTextureName, false); return; }
    m_pShaderManager->setIntValue(g_UseTextureName, true);
    m_pShaderManager->setSampler2DValue(g_TextureValueName, slot);
}
void SceneManager::SetTextureUVScale(float u, float v) {
    if (m_pShaderManager) m_pShaderManager->setVec2Value("UVscale", glm::vec2(u, v));
}
void SceneManager::SetShaderMaterial(std::string tag) {
    if (!m_pShaderManager) return;
    OBJECT_MATERIAL m{};
    if (FindMaterial(tag, m)) {
        m_pShaderManager->setVec3Value("material.ambientColor", m.ambientColor);
        m_pShaderManager->setFloatValue("material.ambientStrength", m.ambientStrength);
        m_pShaderManager->setVec3Value("material.diffuseColor", m.diffuseColor);
        m_pShaderManager->setVec3Value("material.specularColor", m.specularColor);
        m_pShaderManager->setFloatValue("material.shininess", m.shininess);
    }
}

// ---------- prepare ----------
void SceneManager::PrepareScene()
{
    // core meshes we definitely use
    m_basicMeshes->LoadPlaneMesh();
    m_basicMeshes->LoadCylinderMesh();
#if HAS_TORUS_MESH
    m_basicMeshes->LoadTorusMesh();
#endif

    // materials
    m_objectMaterials.clear();

    m_objectMaterials.push_back({ 0.15f, glm::vec3(0.80f,0.75f,0.65f), glm::vec3(1), glm::vec3(1), 64.0f, "shinyWood" }); // table
    m_objectMaterials.push_back({ 0.12f, glm::vec3(0.90f,0.90f,0.95f), glm::vec3(1), glm::vec3(0.25f), 16.0f, "ceramic" });
    m_objectMaterials.push_back({ 0.08f, glm::vec3(0.05f,0.05f,0.06f),  glm::vec3(0.4f), glm::vec3(0.7f), 64.0f, "metal" });
    m_objectMaterials.push_back({ 0.10f, glm::vec3(0.10f,0.10f,0.12f), glm::vec3(0.3f), glm::vec3(0.3f), 8.0f,  "plasticBlack" });
    m_objectMaterials.push_back({ 0.12f, glm::vec3(0.85f,0.85f,0.90f), glm::vec3(0.9f), glm::vec3(0.2f),  8.0f, "plasticWhite" });
    m_objectMaterials.push_back({ 0.10f, glm::vec3(0.20f,0.45f,0.95f), glm::vec3(0.9f), glm::vec3(0.3f), 16.0f, "plasticBlue" });

    // optional textures (falls back to colors if not found)
    CreateGLTexture("../../Utilities/textures/wood.jpg", "wood");
    CreateGLTexture("../../Utilities/textures/wood_diffuse.jpg", "wood2");
    CreateGLTexture("../../Utilities/textures/screen.jpg", "screen");
    CreateGLTexture("../../Utilities/textures/aluminum.jpg", "aluminum");
    CreateGLTexture("../../Utilities/textures/keyboard.jpg", "keyboard");
    BindGLTextures();

    // === build objects ===
    // Table plane is drawn directly in RenderScene (keeps scene scale clear)

    auto mug = AddObject<Mug>();
    mug->position = { 0.0f, 0.0f,  0.0f };

    auto monitor = AddObject<Monitor>();
    monitor->position = { -3.0f, 0.0f, -0.8f };

    auto laptop = AddObject<Laptop>();
    laptop->position = { 2.2f, 0.0f, -0.2f };

    auto block = AddObject<ChargingBlock>();
    block->position = { 0.8f, 0.0f,  2.2f };

    auto phones = AddObject<Headphones>();
    phones->position = { -1.4f, 0.0f,  1.6f };

    auto bottle = AddObject<WaterBottle>();
    bottle->position = { 3.3f, 0.0f,  1.6f };

    // let each object ensure its meshes are ready (safe to call even if already loaded)
    for (auto& obj : m_objects) obj->Prepare(m_basicMeshes);
}

// ---------- render ----------
void SceneManager::RenderScene()
{
    using glm::vec3;

    // Key light — warm (directional)
    m_pShaderManager->setVec3Value("dirLight.direction", vec3(-0.6f, -1.0f, -0.3f));
    m_pShaderManager->setVec3Value("dirLight.color", vec3(1.00f, 0.95f, 0.88f));
    m_pShaderManager->setFloatValue("dirLight.intensity", 1.15f);

    // Fill light — cool (directional)
    m_pShaderManager->setVec3Value("fillLight.direction", vec3(0.7f, -0.6f, 0.6f));
    m_pShaderManager->setVec3Value("fillLight.color", vec3(0.80f, 0.85f, 1.00f));
    m_pShaderManager->setFloatValue("fillLight.intensity", 0.45f);

    // Point light — slightly colored “desk lamp” feel
    m_pShaderManager->setVec3Value("pointLight.position", vec3(1.5f, 1.8f, 1.2f));
    m_pShaderManager->setVec3Value("pointLight.color", vec3(0.95f, 0.90f, 1.00f));
    m_pShaderManager->setFloatValue("pointLight.intensity", 0.9f);
    m_pShaderManager->setVec3Value("pointLight.attenuation", vec3(1.0f, 0.09f, 0.032f)); // const, linear, quad

    // ---------- Table: glossy wood plane ----------
    SetTransformations(vec3(20.0f, 1.0f, 10.0f), 0, 0, 0, vec3(0));
    if (FindTextureSlot("wood") >= 0) SetShaderTexture("wood");
    else if (FindTextureSlot("wood2") >= 0) SetShaderTexture("wood2");
    else SetShaderColor(0.55f, 0.40f, 0.25f, 1.0f);
    SetTextureUVScale(6.0f, 3.0f);
    SetShaderMaterial("shinyWood");
    m_basicMeshes->DrawPlaneMesh();

    // ---------- All objects ----------
    for (auto& obj : m_objects)
        obj->Render(*this, *m_basicMeshes);
}
