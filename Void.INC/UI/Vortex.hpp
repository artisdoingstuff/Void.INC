#pragma once

#include "../Misc/GIncludes.hpp"

struct Vortex {
    sf::VertexArray vortex;
};

inline void updateVortex(Vortex& vortex, sf::Vector2f center, float time, float currentScale) {
    size_t totalVertices = vortex.vortex.getVertexCount();
    const int numArms = 4;
    size_t pointsPerArm = totalVertices / numArms;

    for (int arm = 0; arm < numArms; ++arm) {
        float armOffset = (arm * 2.f * 3.14159f) / numArms;

        for (size_t i = 0; i < pointsPerArm; ++i) {
            size_t idx = arm * pointsPerArm + i;
            float ratio = i / static_cast<float>(pointsPerArm);

            float angle = ratio * 80.f - (time * 0.5f) + armOffset;

            float radius = ratio * 250.f * currentScale;

            float thickness = std::sin(time * 15.f + i) * 3.5f;

            float x = center.x + (radius + thickness) * std::cos(angle);
            float y = center.y + (radius + thickness) * std::sin(angle);

            vortex.vortex[idx].position = { x, y };
            vortex.vortex[idx].color = sf::Color(243, 238, 225, 120);
        }
    }
}