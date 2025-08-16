#pragma once

#include "Upgrades.hpp"

extern vector<UpgradeItem> upgrades;

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
    { "Charming Bubble", 1.03 }, { "Lucky Bubble", 1.07 },
    { "Sudsy Water Balloon", 1.02 }, { "Heated Bubble", 1.02 },
    { "Boiling Bubble", 1.02 }, { "Leafy Bubble", 1.02 },
    { "Autumn Bubble", 1.02 }, { "Sunflower Bubble", 1.02 },
    { "Daisy Bubble", 1.02 }, { "Florescent Bubble", 1.05 },
    { "Chilly Bubble", 1.02 }, { "Frosty Bubble", 1.02 },
    { "Cryogenic Bubble", 1.02 }, { "Inflatable Ducky", 1.02 },
    { "Glistening Bubble", 1.05 }, { "Sparkling Bubble", 1.05 },
    { "Gleaming Bubble", 1.05 }, { "Lucky Golden Bubble", 1.07 },
    { "Fusion Bubble", 1.03 }, { "Fragmented Bubble", 1.03 },
    { "Ionized Bubble", 1.03 }, { "Nuclear Bubble", 1.03 },
    { "Atomic Bubble", 1.03 }, { "Atomized Bubble", 1.03 },
    { "Dark Matter Bubble", 1.03 }, { "Graviton Bubble", 1.03 },
    { "Quantum Foam", 1.03 }, { "Neutrino Bubble", 1.03 },
    { "Void Bubble", 1.03 }, { "Nebula Bubble", 1.04 },
    { "Supernova Bubble", 1.04 }, { "Cosmic Bubble", 1.04 },
    { "Bubbleverse Rift", 1.1 }, { "Timeless Bubble", 1.05 },
    { "Chronofoam", 1.03 }, { "Infinity Bubble", 1.1 },
    { "Hyperclean Bubble", 1.03 }, { "Ultrafoam Reactor", 1.03 },
    { "Transcendent Bubble", 1.05 }, { "Pressure Bubble", 1.03 },
    { "Ultrasoap Bubble", 1.05 }, { "Bubbledrift", 1.04 },
    { "Steam Bubble", 1.03 }, { "Skyfoam Capsule", 1.03 },
    { "Duck of Dawn", 1.05 }, { "Echoing Bubble", 1.11 },
    { "Solar Bubble", 1.04 }, { "Crystal Bubble", 1.03 },
    { "Supercleanser", 1.05 }, { "Dreamweaver Bubble", 1.03 },
    { "Bubble Collider", 1.05 }, { "Polished Bubble", 1.03 },
    { "Sudburst", 1.03 }, { "Nebulized Bubble", 1.03 },
	{ "Bubble Synthesizer", 1.1 }, { "Omnibubble", 1.1 }
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
			"Rawr, this is a soap item upgrade!"
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
    addOtherMilestoneUpgrade(upgrades, "Ionized Bubble", 17500000.0, 15000000.0);
    addOtherMilestoneUpgrade(upgrades, "Nuclear Bubble", 25000000.0, 20000000.0);
    addOtherMilestoneUpgrade(upgrades, "Atomic Bubble", 30000000.0, 25000000.0);
    addOtherMilestoneUpgrade(upgrades, "Atomized Bubble", 30000000.0, 25000000.0);
    addOtherMilestoneUpgrade(upgrades, "Large Loofah", 32500000.0, 27500000.0);
    addOtherMilestoneUpgrade(upgrades, "Dark Matter Bubble", 35000000.0, 30000000.0);
    addOtherMilestoneUpgrade(upgrades, "Graviton Bubble", 40000000.0, 35000000.0);
    addOtherMilestoneUpgrade(upgrades, "Quantum Foam", 50000000.0, 40000000.0);
    addOtherMilestoneUpgrade(upgrades, "Neutrino Bubble", 60000000.0, 50000000.0);
    addOtherMilestoneUpgrade(upgrades, "Void Bubble", 70000000.0, 60000000.0);
    addOtherMilestoneUpgrade(upgrades, "Nebula Bubble", 85000000.0, 70000000.0);
    addOtherMilestoneUpgrade(upgrades, "Supernova Bubble", 100000000.0, 85000000.0);
    addOtherMilestoneUpgrade(upgrades, "Galaxy Soap", 120000000.0, 100000000.0);
    addOtherMilestoneUpgrade(upgrades, "Cosmic Bubble", 140000000.0, 120000000.0);
    addOtherMilestoneUpgrade(upgrades, "Singularity Sud", 160000000.0, 140000000.0);
    addOtherMilestoneUpgrade(upgrades, "Bubbleverse Rift", 200000000.0, 160000000.0);
    addOtherMilestoneUpgrade(upgrades, "Timeless Bubble", 250000000.0, 200000000.0);
    addOtherMilestoneUpgrade(upgrades, "Chronofoam", 300000000.0, 250000000.0);
    addOtherMilestoneUpgrade(upgrades, "Infinity Bubble", 350000000.0, 300000000.0);
    addOtherMilestoneUpgrade(upgrades, "Hyperclean Bubble", 400000000.0, 350000000.0);
    addOtherMilestoneUpgrade(upgrades, "Beyond Clean", 500000000.0, 400000000.0);
    addOtherMilestoneUpgrade(upgrades, "Ultrafoam Reactor", 600000000.0, 500000000.0);
    addOtherMilestoneUpgrade(upgrades, "Transcendent Bubble", 750000000.0, 600000000.0);
    addOtherMilestoneUpgrade(upgrades, "True Soap", 1000000000.0, 750000000.0);
    addOtherMilestoneUpgrade(upgrades, "Pressure Bubble", 1050000000.0, 900000000.0);
    addOtherMilestoneUpgrade(upgrades, "Ultrasoap Bubble", 1200000000.0, 1050000000.0);
    addOtherMilestoneUpgrade(upgrades, "Bubbledrift", 1400000000.0, 1200000000.0);
    addOtherMilestoneUpgrade(upgrades, "Steam Bubble", 1600000000.0, 1400000000.0);
    addOtherMilestoneUpgrade(upgrades, "Skyfoam Capsule", 1800000000.0, 1600000000.0);
    addOtherMilestoneUpgrade(upgrades, "Duck of Dawn", 2100000000.0, 1800000000.0);
    addOtherMilestoneUpgrade(upgrades, "Echoing Bubble", 2400000000.0, 2100000000.0);
    addOtherMilestoneUpgrade(upgrades, "Solar Bubble", 2750000000.0, 2400000000.0);
    addOtherMilestoneUpgrade(upgrades, "Crystal Bubble", 3100000000.0, 2750000000.0);
    addOtherMilestoneUpgrade(upgrades, "Supercleanser", 3500000000.0, 3100000000.0);
    addOtherMilestoneUpgrade(upgrades, "Dreamweaver Bubble", 3900000000.0, 3500000000.0);
    addOtherMilestoneUpgrade(upgrades, "Bubble Collider", 4400000000.0, 3900000000.0);
    addOtherMilestoneUpgrade(upgrades, "Polished Bubble", 4900000000.0, 4400000000.0);
    addOtherMilestoneUpgrade(upgrades, "Sudburst", 5500000000.0, 4900000000.0);
    addOtherMilestoneUpgrade(upgrades, "Nebulized Bubble", 6100000000.0, 5500000000.0);
    addOtherMilestoneUpgrade(upgrades, "Liquid Spectrum", 8300000000.0, 7500000000.0);
    addOtherMilestoneUpgrade(upgrades, "Bubble Synthesizer", 9200000000.0, 8300000000.0);
    addOtherMilestoneUpgrade(upgrades, "Omnibubble", 10000000000.0, 9200000000.0);
}