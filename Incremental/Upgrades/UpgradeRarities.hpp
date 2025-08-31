#pragma once

#include "Upgrades.hpp"

extern vector<UpgradeItem> upgrades;

inline UpgradeRarity getRarityByName(const string& upgradeName)
{
    // Item Rarities
    if (upgradeName == "Loofah") return UpgradeRarity::Uncommon;
    if (upgradeName == "Bubble Bath") return UpgradeRarity::Uncommon;
    if (upgradeName == "Bathtub Jet") return UpgradeRarity::Uncommon;
    if (upgradeName == "Luxury Spa") return UpgradeRarity::Uncommon;
    if (upgradeName == "Foam Pit") return UpgradeRarity::Rare;
    if (upgradeName == "Foam Party") return UpgradeRarity::Rare;
    if (upgradeName == "Sudsy Soap") return UpgradeRarity::Rare;
    if (upgradeName == "Bubble Machine") return UpgradeRarity::Rare;
    if (upgradeName == "Bubbly Pool") return UpgradeRarity::Rare;
    if (upgradeName == "Sparkling Water") return UpgradeRarity::Rare;
    if (upgradeName == "Carbonated Soda") return UpgradeRarity::Rare;
    if (upgradeName == "Bath Bombs") return UpgradeRarity::Epic;
    if (upgradeName == "Bubble Wand") return UpgradeRarity::Epic;

    // Milestone (Item) Rarity: Epic by default
    if (upgradeName.find("Super ") == 0) return UpgradeRarity::Epic;

    // Upgrade Rarities
    if (upgradeName == "Rubber Ducky") return UpgradeRarity::Uncommon;
    if (upgradeName == "Rainbow Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Heart-Shaped Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Duck?") return UpgradeRarity::Rare;
    if (upgradeName == "Golden Bubble") return UpgradeRarity::Rare;
    if (upgradeName == "Fool's Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Misprint Bubble") return UpgradeRarity::Epic;
    if (upgradeName == "Charming Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Lucky Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Florescent Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Inflatable Ducky") return UpgradeRarity::Uncommon;
    if (upgradeName == "Glistening Bubble") return UpgradeRarity::Rare;
    if (upgradeName == "Sparkling Bubble") return UpgradeRarity::Rare;
    if (upgradeName == "Gleaming Bubble") return UpgradeRarity::Rare;
    if (upgradeName == "Lucky Golden Bubble") return UpgradeRarity::Rare;
    if (upgradeName == "Fusion Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Fragmented Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Ionized Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Nuclear Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Atomic Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Atomized Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Large Loofah") return UpgradeRarity::Uncommon;
    if (upgradeName == "Dark Matter Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Graviton Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Quantum Foam") return UpgradeRarity::Uncommon;
    if (upgradeName == "Neutrino Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Void Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Nebula Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Supernova Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Galaxy Soap") return UpgradeRarity::Uncommon;
    if (upgradeName == "Cosmic Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Singularity Sud") return UpgradeRarity::Rare;
    if (upgradeName == "Bubbleverse Rift") return UpgradeRarity::Rare;
    if (upgradeName == "Timeless Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Chronofoam") return UpgradeRarity::Uncommon;
    if (upgradeName == "Infinity Bubble") return UpgradeRarity::Rare;
    if (upgradeName == "Hyperclean Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Beyond Clean") return UpgradeRarity::Rare;
    if (upgradeName == "Ultrafoam Reactor") return UpgradeRarity::Uncommon;
    if (upgradeName == "Transcendent Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "True Soap") return UpgradeRarity::Epic;
    if (upgradeName == "Pressure Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Ultrasoap Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Bubbledrift") return UpgradeRarity::Uncommon;
    if (upgradeName == "Steam Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Skyfoam Capsule") return UpgradeRarity::Uncommon;
    if (upgradeName == "Duck of Dawn") return UpgradeRarity::Rare;
    if (upgradeName == "Echoing Bubble")  return UpgradeRarity::Rare;
    if (upgradeName == "Solar Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Crystal Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Supercleanser") return UpgradeRarity::Uncommon;
    if (upgradeName == "Dreamweaver Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Bubble Collider") return UpgradeRarity::Uncommon;
    if (upgradeName == "Polished Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Sudburst") return UpgradeRarity::Uncommon;
    if (upgradeName == "Nebulized Bubble") return UpgradeRarity::Uncommon;
    if (upgradeName == "Liquid Spectrum") return UpgradeRarity::Rare;
	if (upgradeName == "Bubbe Synthesizer") return UpgradeRarity::Rare;
    if (upgradeName == "Omnibubble") return UpgradeRarity::Legendary;

    return UpgradeRarity::Common;
}
