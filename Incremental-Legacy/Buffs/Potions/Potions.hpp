#pragma once

#include "Misc/Includes.hpp"
#include "Misc/GlobalVariables.hpp"
#include "Buffs/Trader/TradersList.hpp"

struct potionUpgrade {
    float potionMultiplier = 1.0f;
    float potionDuration = 0.0f;
    float potionCost = 0.0f;
};

struct potionsList {
    string potionName;
    string potionDescription;

    int currentLevel = 0;               // Current upgrade level
    bool isPotionUnlocked = false;      // Available for brewing
    bool isPotionBrewed = false;        // Brewed and ready to consume
    bool isPotionActive = false;        // Consumed and running
    int potionsAmount = 0;              // Brewed stock
    float potionRemainingTimer = 0.0f;  // Active timer
	float brewTime = 0.0f;              // Time to brew

    unordered_map<string, int> itemRequirements;

    vector<potionUpgrade> potionLevel;  // Upgrade levels

    function<void(const potionUpgrade&)> onActivate;
    function<void(const potionUpgrade&)> onExpire;

        
    bool canBrew(const vector<traderItemsList>& traderItems, long double playerBubbles) const {
        if (!isPotionUnlocked || isPotionActive) return false;
        if (playerBubbles < potionLevel[currentLevel].potionCost) return false;

        for (const auto& req : itemRequirements) {
            auto it = find_if(traderItems.begin(), traderItems.end(),
                [&](const traderItemsList& item) { return item.name == req.first; });
            if (it == traderItems.end() || it->timesBought < req.second)
                return false;
        }
        return true;
    }

    bool startBrew(vector<traderItemsList>& traderItems, long double& playerBubbles) {
        if (!canBrew(traderItems, playerBubbles)) return false;

        playerBubbles -= potionLevel[currentLevel].potionCost;
        ++potionsAmount;
        isPotionBrewed = true;
        return true;
    }

    bool consumePotion() {
        if (!isPotionBrewed || isPotionActive || potionsAmount <= 0) return false;

        --potionsAmount;
        isPotionBrewed = false;
        isPotionActive = true;
        potionRemainingTimer = potionLevel[currentLevel].potionDuration;

        if (onActivate) onActivate(potionLevel[currentLevel]);

        return true;
    }

    void updatePotion(float dt) {
        if (isPotionActive) {
            potionRemainingTimer -= dt;
            if (potionRemainingTimer <= 0.f) {
                expire();
            }
        }
    }

    bool upgradePotion(long double& playerBubbles) {
        if (currentLevel + 1 >= (int)potionLevel.size()) return false; // max level check
        float potionCost = potionLevel[currentLevel + 1].potionCost;
        if (playerBubbles < potionCost) return false;

        playerBubbles -= potionCost;
        ++currentLevel;
        return true;
    }

    void expire() {
        if (isPotionActive) {
            if (onExpire) onExpire(potionLevel[currentLevel]);
            isPotionActive = false;
            potionRemainingTimer = 0.f;
        }
    }
};

inline void to_json(json& j, const potionUpgrade& u) {
    j = json{
        {"potionMultiplier", u.potionMultiplier},
        {"potionDuration", u.potionDuration},
        {"potionCost", u.potionCost}
    };
}

inline void from_json(const json& j, potionUpgrade& u) {
    j.at("potionMultiplier").get_to(u.potionMultiplier);
    j.at("potionDuration").get_to(u.potionDuration);
    j.at("potionCost").get_to(u.potionCost);
}

inline void to_json(json& j, const potionsList& p) {
    j = json{
        {"potionName", p.potionName},
        {"potionDescription", p.potionDescription},
        {"currentLevel", p.currentLevel},
        {"isPotionUnlocked", p.isPotionUnlocked},
        {"isPotionBrewed", p.isPotionBrewed},
        {"isPotionActive", p.isPotionActive},
        {"potionsAmount", p.potionsAmount},
        {"potionRemainingTimer", p.potionRemainingTimer},
        {"brewTime", p.brewTime},
        {"itemRequirements", p.itemRequirements},
        {"potionLevel", p.potionLevel}
    };
}

inline void from_json(const json& j, potionsList& p) {
    j.at("potionName").get_to(p.potionName);
    j.at("potionDescription").get_to(p.potionDescription);
    j.at("currentLevel").get_to(p.currentLevel);
    j.at("isPotionUnlocked").get_to(p.isPotionUnlocked);
    j.at("isPotionBrewed").get_to(p.isPotionBrewed);
    j.at("isPotionActive").get_to(p.isPotionActive);
    j.at("potionsAmount").get_to(p.potionsAmount);
    j.at("potionRemainingTimer").get_to(p.potionRemainingTimer);
    j.at("brewTime").get_to(p.brewTime);
    j.at("itemRequirements").get_to(p.itemRequirements);
    j.at("potionLevel").get_to(p.potionLevel);
}