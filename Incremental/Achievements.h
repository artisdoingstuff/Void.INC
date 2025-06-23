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

struct Achievement
{
    string name;
    string description;

    AchievementType type;
    long double requirement = 0.0; // Could mean total bubbles, total clicks, etc.
    string targetUpgrade = "";     // Only used for SpecificUpgrade

    bool unlocked = false;

    optional<sf::Sprite> spriteIcon;

    bool checkUnlock(
        long double totalBubbles,
        long double totalClicks,
        const vector<UpgradeItem>& upgrades,
        const unordered_map<string, int>& buffTriggers = {}
    )
    {
        if (unlocked) return false;

        switch (type)
        {
        case AchievementType::TotalBubbles:
            return (totalBubbles >= requirement);

        case AchievementType::Clicks:
            return (totalClicks >= requirement);

        case AchievementType::UpgradeCount:
        {
            long long sum = 0;
            for (const auto& u : upgrades)
                sum += u.count;
            return sum >= requirement;
        }

        case AchievementType::SpecificUpgrade:
        {
            auto it = find_if(upgrades.begin(), upgrades.end(),
                [&](const UpgradeItem& u) {
                    return u.name == targetUpgrade && u.count >= requirement;
                });
            return it != upgrades.end();
        }

        case AchievementType::MilestoneReached:
        {
            int milestoneCount = 0;
            for (const auto& u : upgrades)
                if (u.isMilestone && u.count > 0)
                    ++milestoneCount;
            return milestoneCount >= requirement;
        }

        case AchievementType::BuffTriggered:
        {
            auto it = buffTriggers.find(targetUpgrade);
            return it != buffTriggers.end() && it->second >= requirement;
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
        {"type", static_cast<int>(a.type)},
        {"requirement", a.requirement},
        {"targetUpgrade", a.targetUpgrade},
        {"unlocked", a.unlocked}
    };
}

inline void from_json(const json& j, Achievement& a)
{
    j.at("name").get_to(a.name);
    j.at("description").get_to(a.description);
    a.type = static_cast<AchievementType>(j.at("type").get<int>());
    j.at("requirement").get_to(a.requirement);
    if (j.contains("targetUpgrade"))
        j.at("targetUpgrade").get_to(a.targetUpgrade);
    if (j.contains("unlocked"))
        j.at("unlocked").get_to(a.unlocked);
}