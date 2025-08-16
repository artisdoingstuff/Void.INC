#pragma once

#include "Misc/Includes.hpp"

struct PotionUpgrade {
    int potionLevel = 0;
    int baseCost = 1000;

    PotionUpgrade(int level = 0, int cost = 1000)
        : potionLevel(level), baseCost(cost) {
    }

    int getUpgradeCost() const {
        return static_cast<int>(baseCost * pow(2, potionLevel));
    }

    void upgradePotion() { potionLevel++; }
};

struct Potion {
    string name;
    string description;

    PotionUpgrade potionUpgrade;

    bool isBrewing = false;
    float brewTime = 0.0f;
    float brewProgress = 0.0f;

    float duration = 0.0f;
    float buffMultiplier = 1.0f;

    vector<pair<string, int>> potionRecipes;

    void startBrewing() {
        if (!isBrewing) {
            isBrewing = true;
            brewProgress = 0.0f;
        }
    }

    void updateBrewing(float deltaTime) {
        if (isBrewing) {
            brewProgress += deltaTime;
            if (brewProgress >= brewTime) {
                brewProgress = brewTime;
                isBrewing = false;
            }
        }
    }

    bool isReady() const {
        return (!isBrewing && brewProgress >= brewTime);
    }

    void resetBrewing() {
        isBrewing = false;
        brewProgress = 0.0f;
    }
};

struct PotionStack {
    Potion* potion;
    int quantity = 0;

    PotionStack(Potion* p = nullptr, int qty = 0)
        : potion(p), quantity(qty) {
    }
};