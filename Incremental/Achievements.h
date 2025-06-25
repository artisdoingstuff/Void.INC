#pragma once

#include "Includes.h"
#include "Upgrades.h"

enum class AchievementType
{
    TotalBubbles,
    Clicks,
    UpgradeCount,
    SpecificUpgrade,
    MilestoneReached,
    BuffTriggered
};

struct AchievementPopup {
    string title;
    float elapsed = 0.f;
    float duration = 2.5f;
    bool active = true;
};

struct Achievement
{
    string name;
    string description;

    AchievementType achievementType;
    long double unlockThreshold = 0.0;  // Could mean total bubbles, total clicks, etc.
    string targetUpgrade = "";          // Only used for SpecificUpgrade

    bool isUnlocked = false;

    long double progressValue = 0.0;

    optional<sf::Sprite> spriteIcon;

    bool checkUnlock(
        long double totalBubbles,
        long double totalClicks,
        const vector<UpgradeItem>& upgrades,
        const unordered_map<string, int>& buffTriggers = {}
    )
    {
        if (isUnlocked) return false;

        switch (achievementType)
        {
        case AchievementType::TotalBubbles:
            return (totalBubbles >= unlockThreshold);

        case AchievementType::Clicks:
            return (totalClicks >= unlockThreshold);

        case AchievementType::UpgradeCount:
        {
            long long sum = 0;
            for (const auto& u : upgrades)
                sum += u.count;
            return sum >= unlockThreshold;
        }

        case AchievementType::SpecificUpgrade:
        {
            auto it = find_if(upgrades.begin(), upgrades.end(),
                [&](const UpgradeItem& u) {
                    return u.name == targetUpgrade && u.count >= unlockThreshold;
                });
            return it != upgrades.end();
        }

        case AchievementType::MilestoneReached:
        {
            int milestoneCount = 0;
            for (const auto& u : upgrades)
                if (u.isMilestone && u.count > 0)
                    ++milestoneCount;
            return milestoneCount >= unlockThreshold;
        }

        case AchievementType::BuffTriggered:
        {
            auto it = buffTriggers.find(targetUpgrade);
            return it != buffTriggers.end() && it->second >= unlockThreshold;
        }

        default:
            return false;
        }
    }
};

inline void to_json(json& j, const Achievement& a)
{
    j = json{
        {"name", a.name},
        {"description", a.description},
        {"achievementType", static_cast<int>(a.achievementType)},
        {"unlockThreshold", a.unlockThreshold},
        {"targetUpgrade", a.targetUpgrade},
        {"isUnlocked", a.isUnlocked},
        {"progressValue", a.progressValue}
    };
}

inline void from_json(const json& j, Achievement& a)
{
    j.at("name").get_to(a.name);
    j.at("description").get_to(a.description);
    a.achievementType = static_cast<AchievementType>(j.at("achievementType").get<int>());
    j.at("unlockThreshold").get_to(a.unlockThreshold);

    if (j.contains("targetUpgrade"))
        j.at("targetUpgrade").get_to(a.targetUpgrade);
    if (j.contains("isUnlocked"))
        j.at("isUnlocked").get_to(a.isUnlocked);
    if (j.contains("progressValue"))
        j.at("progressValue").get_to(a.progressValue);
    else
        a.progressValue = 0.0;
}