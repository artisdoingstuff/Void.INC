#pragma once

#include "Achievements/Achievements.hpp"
#include "Includes.hpp"
#include "Marketplace/Trader.hpp"
#include "Upgrades/Upgrades.hpp"
#include "Upgrades/UpgradesList.hpp"
#include "Upgrades/UpgradeRarities.hpp"

extern long double totalUpgradeCount;
extern string gameVersion;

// Save the game version to a JSON file
void versionSave(
    string version
)
{
    json saveVersion;

    saveVersion["version"] = version;

    ofstream file("version.json");

    if (file.is_open())
    {
        file << setw(4) << saveVersion << endl;
        file.close();
        cout << version << endl;
    }
    else
    {
        cerr << "Unable to open version.json for writing." << endl;
    }
}

// Save the game to a JSON file
void saveFileToJson(
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
    
    json traderData = json::array();

    for (const auto& item : traderItems)
        traderData.push_back(item);

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
    saveData["trader"] = traderData;
    saveData["potions"] = potions;

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
    if (saveData.contains("trader"))
    {
        for (const auto& savedTrader : saveData["trader"])
        {
            string name = savedTrader["name"];
            auto it = find_if(traderItems.begin(), traderItems.end(), [&](traderItemsList& item) {
                return item.name == name;
                });
            
            if (it != traderItems.end())
            {
                from_json(savedTrader, *it);
                // Recalculate current cost from saved timesBought
                it->cost = round(it->baseCost * pow(itemInflationRate, it->timesBought));
            }
        }
    }

    if (saveData.contains("potions")) {
        vector<potionsList> loadedPotions = saveData["potions"].get<vector<potionsList>>();
        for (auto& saved : loadedPotions) {
            auto it = find_if(potions.begin(), potions.end(), [&](potionsList& p) {
                return p.potionName == saved.potionName;
                });

            if (it != potions.end()) {
                *it = saved;
                it->onActivate = saved.onActivate;
                it->onExpire = saved.onExpire;
            }
        }
    }

    file.close();
    cout << "Game loaded from save_file.json" << endl;
}
