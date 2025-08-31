#pragma once

#include "Misc/Includes.hpp"
#include "Upgrades/Upgrades.hpp"

struct MutatedBubbleBuff {
    bool backfire = false;
    int affectedItemIndex = -1;
    float multiplier = 1.0f;

    void activate(const std::vector<UpgradeItem>& upgrades) {
        float roll = static_cast<float>(rand()) / RAND_MAX;
        backfire = (roll <= 0.2f);

        if (!backfire) {
            std::vector<int> ownedIndices;
            for (int i = 0; i < static_cast<int>(upgrades.size()); ++i) {
                if (upgrades[i].isItemUpgrade && upgrades[i].count > 0) {
                    ownedIndices.push_back(i);
                }
            }

            if (!ownedIndices.empty()) {
                int rnd = rand() % ownedIndices.size();
                affectedItemIndex = ownedIndices[rnd];
                multiplier = 1.f + 0.2f * static_cast<float>(upgrades[affectedItemIndex].count);
            }
            else {
                backfire = true;
                multiplier = 0.5f;
            }
        }
        else {
            multiplier = 0.5f;
        }
    }

    float getMultiplier() const {
        return multiplier;
    }
};

