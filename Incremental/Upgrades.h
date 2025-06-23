#pragma once

#include "Includes.h"

extern const long double shopInflationMultiplier;
extern long double totalUpgradeCount;
extern long double bubblesPerSecond;

struct UpgradeItem
{
    string name;
    int count = 0;

    long double baseCost;
    long double currentCost;

    long double baseProduction; // bubbles per second
    long double unlockThreshold;

    bool isMilestone = false;
    bool unlockedByMilestone = false;
    long double milestoneTriggerValue = 0.0; // milestone threshold

    bool isItemUpgrade = false;
    bool isOtherUpgrade = false;

    bool isClickUpgrade = false;
    bool isDurationUpgrade = false;
    bool isMinorUpgrade = false;
    bool isMajorUpgrade = false;

    optional<sf::Sprite> spriteUpgrade;

    void updateCost(const long double inflation = shopInflationMultiplier)
    {
        currentCost = round(baseCost * pow(inflation, count));
    }

    long double getProduction() const
    {
        return count * baseProduction;
    }

    bool isUnlocked(long double currentBubbles, const vector<UpgradeItem>& upgrades) const
    {
        if (!unlockedByMilestone)
            return currentBubbles >= unlockThreshold;

        if (!isMilestone)
            return true;

        // Only for milestone upgrades with unlockedByMilestone = true
        string baseName = name.substr(6); // remove "Super "
        size_t tierPos = baseName.find(" Tier ");
        if (tierPos != string::npos)
            baseName = baseName.substr(0, tierPos);

        auto it = find_if(upgrades.begin(), upgrades.end(), [&](const UpgradeItem& u) {
            return u.name == baseName;
            });

        return it != upgrades.end() && it->count >= milestoneTriggerValue;
    }

    bool canAfford(long double currentBubbles) const
    {
        return currentBubbles >= currentCost;
    }

    void purchase(long double& currentBubbles, const long double inflation = shopInflationMultiplier)
    {
        if (!canAfford(currentBubbles))
            return;
        if (isMilestone && count >= 1)
            return;

        currentBubbles -= currentCost;
        count++;
        totalUpgradeCount++;
        updateCost(inflation);
    }
};

inline void generateItemMilestoneUpgrades(
    vector<UpgradeItem>& upgrades,
    const string& baseName,
    long double baseBuildingCost
)
{
    const vector<int> thresholds = {
        10, 25, 50, 75, 100, 150, 200, 250
    };

    for (size_t i = 0; i < thresholds.size(); ++i)
    {
        int threshold = thresholds[i];

        long double buildingCost = baseBuildingCost * pow(shopInflationMultiplier, threshold);
        long double floorVal = floor(log10(buildingCost));
        long double raisedVal = pow(10, floorVal);

        long double milestoneCost = ceil(buildingCost / raisedVal) * raisedVal;

        upgrades.push_back(
            {
                "Super " + baseName + " Tier " + to_string(i + 1), // name
                0,                                  // count
                milestoneCost,                      // baseCost
                milestoneCost,                      // currentCost
                0.0,                               // baseProduction
                0.0,                               // unlockThreshold
                true,                               // isMilestone
                true,                               // unlockedByMilestone
                static_cast<long double>(threshold),// milestoneTriggerValue
                true                                // isItemUpgrade
            }
        );
    }
}

inline bool hasUpgrade(const vector<UpgradeItem>& upgrades, const string& name)
{
    return any_of(upgrades.begin(), upgrades.end(), [&](const UpgradeItem& u)
        {
            return u.name == name && u.count > 0;
        }
    );
}

inline int getUpgradeItemMilestoneCount(const string& baseName, const vector<UpgradeItem>& upgrades)
{
    return count_if(upgrades.begin(), upgrades.end(), [&](const UpgradeItem& u)
        {
            return u.isMilestone && u.count > 0 && u.name.find("Super " + baseName + " Tier ") == 0;
        }
    );
}

inline void addOtherMilestoneUpgrade(
    vector<UpgradeItem>& upgrades,
    const string& name,
    long double unlockAt,
    long double cost
)
{
    upgrades.push_back(
        {
            name,
            0,
            cost,
            cost,
            0.0,                // No base production
            unlockAt,           // Unlock condition (based on allTimeBubbles)
            false,
            false,
            0.0,
            false,
            true,               // isOtherUpgrade True
            false,
            false,
            false,
            false
        }
    );
}

inline const unordered_map<string, long double> globalUpgradeMultiplierValues = {
        { "Red Bubble", 1.01 }, { "Green Bubble", 1.01 }, { "Blue Bubble", 1.01 },
        { "Rubber Ducky", 1.01 }, { "Rainbow Bubble", 1.02 },
        { "Cyan Bubble", 1.01 }, { "Indigo Bubble", 1.01 },
        { "Heart-Shaped Bubble", 1.02 }, { "Bath Bubbles", 1.02 },
        { "Detergent Bubbles", 1.02 }, { "Duck?", 1.01 },
        { "Carbonation", 1.02 }, { "Golden Bubble", 1.10 },
        { "Fool's Bubble", 1.01 }, { "Misprint Bubble", 1.20 },
        { "Spring Bubble", 1.02 }, { "Cherry Bubble", 1.02 },
        { "Blossoming Bubble", 1.02 }, { "Rose Bubble", 1.02 },
        { "Dandelion Bubble", 1.02 }, { "Charming Bubble", 1.02 },
        { "Lucky Bubble", 1.07 }, { "Sudsy Water Balloon", 1.02 }
};

inline long double getBuffedProduction(const UpgradeItem& u, const vector<UpgradeItem>& upgrades)
{
    long double production = u.baseProduction;

    int itemMilestoneCount = getUpgradeItemMilestoneCount(u.name, upgrades);

    if (itemMilestoneCount > 0)
        if (u.isItemUpgrade)
            production *= pow(1.8, itemMilestoneCount);

    return production * u.count;
}

inline void to_json(json& j, const UpgradeItem& u)
{
    j = json{
        {"name", u.name},
        {"count", u.count},
        {"baseCost", u.baseCost},
        {"baseProduction", u.baseProduction},
        {"unlockThreshold", u.unlockThreshold},
        {"isMilestone", u.isMilestone},
        {"unlockedByMilestone", u.unlockedByMilestone},
        {"milestoneTriggerValue", u.milestoneTriggerValue},
        {"isItemUpgrade", u.isItemUpgrade},
        {"isOtherUpgrade", u.isOtherUpgrade},
        {"isClickUpgrade", u.isClickUpgrade},
        {"isDurationUpgrade", u.isDurationUpgrade},
        {"isMinorUpgrade", u.isMinorUpgrade},
        {"isMajorUpgrade", u.isMajorUpgrade}
    };
}

inline void from_json(const json& j, UpgradeItem& u)
{
    j.at("name").get_to(u.name);
    j.at("count").get_to(u.count);
    j.at("baseCost").get_to(u.baseCost);
    j.at("baseProduction").get_to(u.baseProduction);
    j.at("unlockThreshold").get_to(u.unlockThreshold);
    if (j.contains("isMilestone")) j.at("isMilestone").get_to(u.isMilestone);
    if (j.contains("unlockedByMilestone")) j.at("unlockedByMilestone").get_to(u.unlockedByMilestone);
    if (j.contains("milestoneTriggerValue")) j.at("milestoneTriggerValue").get_to(u.milestoneTriggerValue);
    if (j.contains("isItemUpgrade")) j.at("isItemUpgrade").get_to(u.isItemUpgrade);
    if (j.contains("isOtherUpgrade")) j.at("isOtherUpgrade").get_to(u.isOtherUpgrade);
    if (j.contains("isClickUpgrade")) j.at("isClickUpgrade").get_to(u.isClickUpgrade);
    if (j.contains("isDurationUpgrade")) j.at("isDurationUpgrade").get_to(u.isDurationUpgrade);
    if (j.contains("isMinorUpgrade")) j.at("isMinorUpgrade").get_to(u.isMinorUpgrade);
    if (j.contains("isMajorUpgrade")) j.at("isMajorUpgrade").get_to(u.isMajorUpgrade);

    u.updateCost();
}