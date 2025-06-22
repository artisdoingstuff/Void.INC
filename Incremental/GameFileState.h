#pragma once

#include "Includes.h"
#include "Upgrades.h"

extern long double totalUpgradeCount;
extern string gameVersion;

// Function to save the game state to a file
void saveFileToJson(
    string version,
    time_t timestamp,
    long double duckCounter,
    long double bubbles,
    long double allTimeBubbles,
    long double allTimeBubblesPerClick,
    long double baseBubblesPerClick,
    long double clickMultiplier,
    long double bubblesPerSecond,
    vector<UpgradeItem>& upgrades
)
{
    json saveData;

    auto round2 = [](long double val) -> long double {
        return round(val * 100.0) / 100.0;
        };

    saveData["version"] = version;
    saveData["timestamp"] = timestamp;
    saveData["duckCounter"] = round2(duckCounter);
    saveData["bubbles"] = round2(bubbles);
    saveData["allTimeBubbles"] = round2(allTimeBubbles);
    saveData["allTimeBubblesPerClick"] = round2(allTimeBubblesPerClick);
    saveData["baseBubblesPerClick"] = round2(baseBubblesPerClick);
    saveData["clickMultiplier"] = round2(clickMultiplier);
    saveData["bubblesPerSecond"] = round2(bubblesPerSecond);
    saveData["totalUpgradeCount"] = round2(totalUpgradeCount);
    saveData["upgrades"] = upgrades;

    ofstream file("save_file.json");

    if (file.is_open())
    {
        file << setw(4) << saveData << endl;
        file.close();
        cout << "Game saved successfully." << endl;
    }
    else
    {
        cerr << "Unable to open save_file.json for writing." << endl;
    }
}

void loadFileFromJson(
    time_t& timestamp,
    long double& duckCounter,
    long double& bubbles,
    long double& allTimeBubbles,
    long double& allTimeBubblesPerClick,
    long double& baseBubblesPerClick,
    long double& clickMultiplier,
    long double& bubblesPerSecond,
    vector<UpgradeItem>& upgrades,
    const map<string, sf::Texture>& upgradeTextures
)
{
    ifstream file("save_file.json");
    if (!file.is_open())
    {
        cerr << "No save file found. Starting new game." << endl;
        timestamp = time(nullptr);
        totalUpgradeCount = 0;
        return;
    }

    json saveData;
    file >> saveData;

    timestamp = saveData["timestamp"];
    duckCounter = saveData["duckCounter"];
    bubbles = saveData["bubbles"];
    allTimeBubbles = saveData["allTimeBubbles"];
    allTimeBubblesPerClick = saveData["allTimeBubblesPerClick"];
    baseBubblesPerClick = saveData["baseBubblesPerClick"];
    clickMultiplier = saveData["clickMultiplier"];
    bubblesPerSecond = saveData["bubblesPerSecond"];

    if (saveData.contains("totalUpgradeCount"))
        totalUpgradeCount = saveData["totalUpgradeCount"];
    else
        totalUpgradeCount = 0;

    vector<UpgradeItem> savedUpgrades = saveData["upgrades"].get<vector<UpgradeItem>>();

    for (const auto& saved : savedUpgrades)
    {
        auto it = find_if(upgrades.begin(), upgrades.end(), [&](UpgradeItem& u) {
            return u.name == saved.name;
            });

        if (it != upgrades.end())
        {
            it->count = saved.count;
            it->baseCost = saved.baseCost;
            it->currentCost = saved.currentCost;
            it->baseProduction = saved.baseProduction;
            it->unlockThreshold = saved.unlockThreshold;

            it->isMilestone = saved.isMilestone;
            it->unlockedByMilestone = saved.unlockedByMilestone;
            it->milestoneTriggerValue = saved.milestoneTriggerValue;

            it->isItemUpgrade = saved.isItemUpgrade;
            it->isOtherUpgrade = saved.isOtherUpgrade;
            it->isClickUpgrade = saved.isClickUpgrade;
            it->isDurationUpgrade = saved.isDurationUpgrade;
            it->isMinorUpgrade = saved.isMinorUpgrade;
            it->isMajorUpgrade = saved.isMajorUpgrade;

            it->updateCost(); // in case inflation changed
        }
    }

    for (auto& upgrade : upgrades)
    {
        auto it = upgradeTextures.find(upgrade.name);
        if (it != upgradeTextures.end())
        {
            upgrade.spriteUpgrade = sf::Sprite(it->second);
        }
        else
        {
            upgrade.spriteUpgrade.reset();
        }
    }

    file.close();
    cout << "Game loaded from save_file.json" << endl;
}