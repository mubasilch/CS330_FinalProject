#pragma once
#include "Renderable.h"

class MugAndSaucer : public Renderable {
public:
    MugAndSaucer() = default;
    void Prepare(ISceneContext& /*ctx*/) override {}
    void Render(ISceneContext& ctx) override;
};
