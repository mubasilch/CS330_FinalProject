#pragma once
#include "Renderable.h"
#include <glm/glm.hpp>
#include <string>

class TablePlane : public Renderable {
public:
    // configurable constructor (defaults match your current scene)
    TablePlane(glm::vec3 scale = {20.f, 1.f, 10.f},
               glm::vec3 pos   = {0.f, 0.f, 0.f},
               std::string woodTagPrimary   = "wood",
               std::string woodTagFallback  = "wood2",
               std::string materialTag      = "shinyWood",
               glm::vec2   uvScale          = {6.f, 3.f})
        : m_scale(scale), m_pos(pos),
          m_woodTag(woodTagPrimary),
          m_woodTagFallback(woodTagFallback),
          m_materialTag(materialTag),
          m_uv(uvScale) {}

    void Prepare(ISceneContext& /*ctx*/) override {}
    void Render(ISceneContext& ctx) override;

private:
    glm::vec3 m_scale;
    glm::vec3 m_pos;
    std::string m_woodTag;
    std::string m_woodTagFallback;
    std::string m_materialTag;
    glm::vec2   m_uv;
};
