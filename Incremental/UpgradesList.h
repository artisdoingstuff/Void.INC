#pragma once

#include "Upgrades.h"

extern vector<UpgradeItem> upgrades;

inline UpgradeRarity getRarityByName(const string& upgradeName)
{
	if (upgradeName == "Toothpaste")        return UpgradeRarity::Uncommon;
	if (upgradeName == "Loofah")            return UpgradeRarity::Uncommon;
	if (upgradeName == "Bubble Bath")       return UpgradeRarity::Uncommon;
	if (upgradeName == "Bathtub Jet")       return UpgradeRarity::Uncommon;
	if (upgradeName == "Luxury Spa")        return UpgradeRarity::Rare;
	if (upgradeName == "Foam Pit")          return UpgradeRarity::Rare;
	if (upgradeName == "Foam Party")        return UpgradeRarity::Rare;
	if (upgradeName == "Sudsy Soap")        return UpgradeRarity::Rare;
	if (upgradeName == "Bubble Machine")    return UpgradeRarity::Rare;
    if (upgradeName == "Bubbly Pool")       return UpgradeRarity::Epic;
	if (upgradeName == "Sparkling Water")   return UpgradeRarity::Epic;
	if (upgradeName == "Carbonated Soda")   return UpgradeRarity::Epic;
	if (upgradeName == "Bath Bombs")        return UpgradeRarity::Epic;
	if (upgradeName == "Bubble Wand")       return UpgradeRarity::Epic;
    if (upgradeName.find("Super ") == 0)    return UpgradeRarity::Epic;

    return UpgradeRarity::Common;
}

inline const unordered_map<string, long double> globalUpgradeMultiplierValues = {
    { "Red Bubble", 1.01 }, { "Green Bubble", 1.01 },
    { "Blue Bubble", 1.01 }, { "Rubber Ducky", 1.01 },
    { "Rainbow Bubble", 1.02 }, { "Cyan Bubble", 1.01 },
    { "Indigo Bubble", 1.01 }, { "Heart-Shaped Bubble", 1.02 },
    { "Bath Bubbles", 1.02 }, { "Detergent Bubbles", 1.02 },
    { "Duck?", 1.01 }, { "Carbonation", 1.02 },
    { "Golden Bubble", 1.10 }, { "Fool's Bubble", 1.01 },
    { "Misprint Bubble", 1.125 }, { "Spring Bubble", 1.02 },
    { "Cherry Bubble", 1.02 }, { "Blossoming Bubble", 1.02 },
    { "Rose Bubble", 1.02 }, { "Dandelion Bubble", 1.02 },
    { "Charming Bubble", 1.02 }, { "Lucky Bubble", 1.07 },
    { "Sudsy Water Balloon", 1.02 }, { "Heated Bubble", 1.02 },
    { "Boiling Bubble", 1.02 }, { "Leafy Bubble", 1.02 },
    { "Autumn Bubble", 1.02 }, { "Sunflower Bubble", 1.02 },
    { "Daisy Bubble", 1.02 }, { "Florescent Bubble", 1.05 },
    { "Chilly Bubble", 1.02 }, { "Frosty Bubble", 1.02 },
    { "Cryogenic Bubble", 1.02 }, { "Inflatable Ducky", 1.02 },
    { "Glistening Bubble", 1.05 }, { "Sparkling Bubble", 1.05 },
    { "Gleaming Bubble", 1.05 }, { "Lucky Golden Bubble", 1.07 },
    { "Fusion Bubble", 1.03 }, { "Fragmented Bubble", 1.03 }
};

inline void upgradesList()
{
    // Items
    upgrades.push_back(
        {
            "Soap",                 // Reference Name
            0,                      // Item count
            10.0, 10.0,             // Item base/current cost
            0.15,                   // Item production (bubbles per second)
            10.0,                   // Unlock threshold
            false,                  // isMilestone
            false,                  // unlockedByMilestone
            0.0,                    // milestoneTriggerValue
            true,                   // isitemUpgrade <-- This is what matters for Items
            false,                  // isOtherUpgrade <-- ONLY for non-item upgrades (such as Bubble Milestones)
            true,                   // isClickUpgrade <-- True only for Soap
            false,                  // isDurationUpgrade <-- Doesn't affect buff duration
            true,                   // isMinorUpgrade <-- Will have Minor Upgrades (small buffs)
            true,                   // isMajorUpgrade <-- Will have Major Upgrades (big buffs)
            UpgradeRarity::Common   // Rarity
        }
    );
    upgrades.push_back({ "Hand Wash", 0, 75.0, 75.0, 0.5, 100.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Shampoo", 0, 250.0, 250.0, 1.0, 350.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Shaving Foam", 0, 1000.0, 1000.0, 2.5, 1200.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Toothpaste", 0, 3000.0, 3000.0, 5.0, 3500.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Loofah", 0, 7000.0, 7000.0, 8.0, 7500.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Bubble Bath", 0, 18000.0, 18000.0, 15.0, 20000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Bathtub Jet", 0, 40000.0, 40000.0, 22.0, 50000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Luxury Spa", 0, 100000.0, 100000.0, 35.0, 150000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Foam Pit", 0, 150000.0, 150000.0, 50.0, 200000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Foam Party", 0, 250000.0, 250000.0, 75.0, 500000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Sudsy Soap", 0, 400000.0, 400000.0, 130.0, 750000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Bubble Machine", 0, 750000.0, 750000.0, 200.0, 1000000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Bubbly Pool", 0, 1200000.0, 1200000.0, 300.0, 1500000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Sparkling Water", 0, 1800000.0, 1800000.0, 450.0, 2200000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Carbonated Soda", 0, 2500000.0, 2500000.0, 650.0, 3000000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Bath Bombs", 0, 4000000.0, 4000000.0, 900.0, 5000000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Bubble Wand", 0, 6000000.0, 6000000.0, 1200.0, 7500000.0, false, false, 0.0, true, false, false, false, true, true });

    // Item Upgrades
    generateItemMilestoneUpgrades(upgrades, "Soap", 10.0);
    generateItemMilestoneUpgrades(upgrades, "Hand Wash", 75.0);
    generateItemMilestoneUpgrades(upgrades, "Shampoo", 250.0);
    generateItemMilestoneUpgrades(upgrades, "Shaving Foam", 1000.0);
    generateItemMilestoneUpgrades(upgrades, "Toothpaste", 3000.0);
    generateItemMilestoneUpgrades(upgrades, "Loofah", 7000.0);
    generateItemMilestoneUpgrades(upgrades, "Bubble Bath", 18000.0);
    generateItemMilestoneUpgrades(upgrades, "Bathtub Jet", 40000.0);
    generateItemMilestoneUpgrades(upgrades, "Luxury Spa", 100000.0);
    generateItemMilestoneUpgrades(upgrades, "Foam Pit", 150000.0);
    generateItemMilestoneUpgrades(upgrades, "Foam Party", 250000.0);
    generateItemMilestoneUpgrades(upgrades, "Sudsy Soap", 400000.0);
    generateItemMilestoneUpgrades(upgrades, "Bubble Machine", 750000.0);
    generateItemMilestoneUpgrades(upgrades, "Bubbly Pool", 1200000.0);
    generateItemMilestoneUpgrades(upgrades, "Sparkling Water", 1800000.0);
    generateItemMilestoneUpgrades(upgrades, "Carbonated Soda", 2500000.0);
    generateItemMilestoneUpgrades(upgrades, "Bath Bombs", 4000000.0);
    generateItemMilestoneUpgrades(upgrades, "Bubble Wand", 6000000.0);

    // Other Upgrades
    addOtherMilestoneUpgrade(
        upgrades,       // Upgrade Call
        "Red Bubble",   // Reference Name
        100.0,          // Upgrade Unlock Threshold
        250.0           // Upgrade Cost
    );
    addOtherMilestoneUpgrade(upgrades, "Green Bubble", 250.0, 500.0);
    addOtherMilestoneUpgrade(upgrades, "Blue Bubble", 500.0, 750.0);
    addOtherMilestoneUpgrade(upgrades, "Rubber Ducky", 750.0, 1000.0);
    addOtherMilestoneUpgrade(upgrades, "Rainbow Bubble", 1000.0, 1500.0);
    addOtherMilestoneUpgrade(upgrades, "Cyan Bubble", 1500.0, 2500.0);
    addOtherMilestoneUpgrade(upgrades, "Indigo Bubble", 2500.0, 4000.0);
    addOtherMilestoneUpgrade(upgrades, "Heart-Shaped Bubble", 5000.0, 7500.0);
    addOtherMilestoneUpgrade(upgrades, "Soap Dispenser", 7500.0, 10000.0);
    addOtherMilestoneUpgrade(upgrades, "Bath Bubbles", 10000.0, 15000.0);
    addOtherMilestoneUpgrade(upgrades, "Detergent Bubbles", 15000.0, 20000.0);
    addOtherMilestoneUpgrade(upgrades, "Duck?", 20000.0, 10000.0);
    addOtherMilestoneUpgrade(upgrades, "Carbonation", 25000.0, 35000.0);
    addOtherMilestoneUpgrade(upgrades, "Golden Bubble", 40000.0, 100000.0);
    addOtherMilestoneUpgrade(upgrades, "Hand Wash Refiller", 50000.0, 75000.0);
    addOtherMilestoneUpgrade(upgrades, "Fool's Bubble", 75000.0, 90000.0);
    addOtherMilestoneUpgrade(upgrades, "Misprint Bubble", 100000.0, 250000.0);
    addOtherMilestoneUpgrade(upgrades, "Spring Bubble", 150000.0, 250000.0);
    addOtherMilestoneUpgrade(upgrades, "Cherry Bubble", 200000.0, 300000.0);
    addOtherMilestoneUpgrade(upgrades, "Blossoming Bubble", 250000.0, 350000.0);
    addOtherMilestoneUpgrade(upgrades, "Rose Bubble", 300000.0, 400000.0);
    addOtherMilestoneUpgrade(upgrades, "Dandelion Bubble", 350000.0, 450000.0);
    addOtherMilestoneUpgrade(upgrades, "Charming Bubble", 400000.0, 650000.0);
    addOtherMilestoneUpgrade(upgrades, "Lucky Bubble", 450000.0, 777777.0);
    addOtherMilestoneUpgrade(upgrades, "Lavender Shampoo", 500000.0, 850000.0);
    addOtherMilestoneUpgrade(upgrades, "Sudsy Water Balloon", 600000.0, 900000.0);
    addOtherMilestoneUpgrade(upgrades, "Heated Bubble", 750000.0, 1000000.0);
    addOtherMilestoneUpgrade(upgrades, "Boiling Bubble", 850000.0, 1100000.0);
    addOtherMilestoneUpgrade(upgrades, "Leafy Bubble", 1000000.0, 1200000.0);
    addOtherMilestoneUpgrade(upgrades, "Autumn Bubble", 1250000.0, 1500000.0);
    addOtherMilestoneUpgrade(upgrades, "Sunflower Bubble", 1500000.0, 1800000.0);
    addOtherMilestoneUpgrade(upgrades, "Daisy Bubble", 1800000.0, 2000000.0);
    addOtherMilestoneUpgrade(upgrades, "Florescent Bubble", 2100000.0, 2400000.0);
    addOtherMilestoneUpgrade(upgrades, "Green Shaving Foam", 2500000.0, 3000000.0);
    addOtherMilestoneUpgrade(upgrades, "Chilly Bubble", 3000000.0, 4000000.0);
    addOtherMilestoneUpgrade(upgrades, "Frosty Bubble", 3500000.0, 4750000.0);
    addOtherMilestoneUpgrade(upgrades, "Cryogenic Bubble", 4000000.0, 5500000.0);
    addOtherMilestoneUpgrade(upgrades, "Inflatable Ducky", 4500000.0, 6000000.0);
    addOtherMilestoneUpgrade(upgrades, "Glistening Bubble", 5000000.0, 6500000.0);
    addOtherMilestoneUpgrade(upgrades, "Sparkling Bubble", 5500000.0, 7000000.0);
    addOtherMilestoneUpgrade(upgrades, "Gleaming Bubble", 6000000.0, 7500000.0);
    addOtherMilestoneUpgrade(upgrades, "Lucky Golden Bubble", 6500000.0, 7777777.0);
    addOtherMilestoneUpgrade(upgrades, "Fusion Bubble", 7500000.0, 8000000.0);
    addOtherMilestoneUpgrade(upgrades, "Fragmented Bubble", 8500000.0, 10000000.0);
    addOtherMilestoneUpgrade(upgrades, "More Toothpaste!", 10000000.0, 12000000.0);
    addOtherMilestoneUpgrade(upgrades, "Blue Soap Bar", 12000000.0, 14000000.0);
}