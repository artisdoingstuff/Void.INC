#pragma once

#include "Achievements.h"
#include "Includes.h"
#include "Marketplace.h"
#include "Upgrades.h"
#include "UpgradesList.h"
#include "UpgradeRarities.h"

extern long double totalUpgradeCount;
extern string gameVersion;

// Save the game to a JSON file
void saveFileToJson(
    string version,
    time_t timestamp,
    long double duckCounter,
    long double buffCounter,
    long double bubbles,
    long double allTimeBubbles,
    long double allTimeBubblesPerClick,
    long double baseBubblesPerClick,
    long double clickMultiplier,
    long double bubblesPerSecond,
    vector<UpgradeItem>& upgrades,
    vector<Achievement>& achievements
)
{
    json saveData;

    auto round2 = [](long double val) -> long double {
        return round(val * 100.0) / 100.0;
        };
    
    json marketplaceData = json::array();

    for (const auto& item : marketplaceItems)
        marketplaceData.push_back(item);

    saveData["version"] = version;
    saveData["timestamp"] = timestamp;
    saveData["duckCounter"] = round2(duckCounter);
    saveData["buffCounter"] = round2(buffCounter);
    saveData["bubbles"] = round2(bubbles);
    saveData["allTimeBubbles"] = round2(allTimeBubbles);
    saveData["allTimeBubblesPerClick"] = round2(allTimeBubblesPerClick);
    saveData["baseBubblesPerClick"] = round2(baseBubblesPerClick);
    saveData["clickMultiplier"] = round2(clickMultiplier);
    saveData["bubblesPerSecond"] = round2(bubblesPerSecond);
    saveData["totalUpgradeCount"] = round2(totalUpgradeCount);
    saveData["upgrades"] = upgrades;
    saveData["gameAchievements"] = achievements;
    saveData["marketplace"] = marketplaceData;

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

// Load the game from a JSON file
void loadFileFromJson(
    time_t& timestamp,
    long double& duckCounter,
    long double& buffCounter,
    long double& bubbles,
    long double& allTimeBubbles,
    long double& allTimeBubblesPerClick,
    long double& baseBubblesPerClick,
    long double& clickMultiplier,
    long double& bubblesPerSecond,
    vector<UpgradeItem>& upgrades,
    const map<string, sf::Texture>& upgradeTextures,
    vector<Achievement>& achievements
)
{
    ifstream file("save_file.json", ios::in);
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
    buffCounter = saveData["buffCounter"];
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

    // Load upgrades
    vector<UpgradeItem> savedUpgrades = saveData["upgrades"].get<vector<UpgradeItem>>();
    for (const auto& saved : savedUpgrades)
    {
        auto it = find_if(upgrades.begin(), upgrades.end(), [&](UpgradeItem& u) {
            return u.name == saved.name;
            });

        if (it != upgrades.end())
        {
            it->copyFromSave(saved);
            it->rarity = getRarityByName(it->name);
            it->updateCost(); // Apply inflation if needed
        }
    }

    // Restore sprites
    for (auto& upgrade : upgrades)
    {
        auto it = upgradeTextures.find(upgrade.name);
        if (it != upgradeTextures.end())
            upgrade.spriteUpgrade = sf::Sprite(it->second);
        else
            upgrade.spriteUpgrade.reset();
    }

    // Load achievements
    if (saveData.contains("gameAchievements"))
    {
        vector<Achievement> loadedAchievements = saveData["gameAchievements"].get<vector<Achievement>>();
        for (const auto& saved : loadedAchievements)
        {
            auto it = find_if(achievements.begin(), achievements.end(), [&](Achievement& a) {
                return a.name == saved.name;
                });

            if (it != achievements.end())
            {
                it->isUnlocked = saved.isUnlocked;
            }
        }

        perkManager = PerkManager();
        for (const auto& a : achievements)
        {
            if (a.isUnlocked)
            {
                PerkEffect effect = getPerkEffectFromAchievementType(a.achievementType);
                perkManager.applyPerk(effect.type, effect.value);
            }
        }
    }

    // Load Buff Shop
    if (saveData.contains("marketplace"))
    {
        for (const auto& savedMarketplace : saveData["marketplace"])
        {
            string name = savedMarketplace["name"];
            auto it = find_if(marketplaceItems.begin(), marketplaceItems.end(), [&](marketplaceItemsList& item) {
                return item.name == name;
                });
            
            if (it != marketplaceItems.end())
            {
                from_json(savedMarketplace, *it);
                // Recalculate current cost from saved timesBought
                it->cost = round(it->baseCost * pow(itemInflationRate, it->timesBought));
            }
        }
    }

    file.close();
    cout << "Game loaded from save_file.json" << endl;
}
