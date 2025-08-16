#pragma once

#include "Includes.hpp"

inline void loadUpgradeTextures(map<string, sf::Texture>& upgradeTextures)
{
    upgradeTextures["Soap"].loadFromFile("Assets/soap_upgrade.png");
    upgradeTextures["Hand Wash"].loadFromFile("Assets/handwash_upgrade.png");
    upgradeTextures["Shampoo"].loadFromFile("Assets/shampoo_upgrade.png");
    upgradeTextures["Shaving Foam"].loadFromFile("Assets/shaving_foam_upgrade.png");
    upgradeTextures["Toothpaste"].loadFromFile("Assets/toothpaste_upgrade.png");
    upgradeTextures["Loofah"].loadFromFile("Assets/loofah_upgrade.png");
}

inline void loadAchievementTexture(map<string, sf::Texture>& achievementTextures)
{
    achievementTextures["It Begins."].loadFromFile("Assets/Achievements/achievement_bubble_1.png");
    achievementTextures["Bubble Beginner"].loadFromFile("Assets/Achievements/achievement_bubble_100.png");
    achievementTextures["Bubble Novice"].loadFromFile("Assets/Achievements/achievement_bubble_1000.png");
    achievementTextures["Locked"].loadFromFile("Assets/Achievements/locked.png");
}

inline sf::Texture bubbleTexture("Assets/Buffs/bubble.png");
inline sf::Texture goldenBubbleTexture("Assets/Buffs/golden_bubble.png");
inline sf::Texture shootingStarTexture("Assets/Buffs/shooting_star.png");

inline sf::Texture achievementIconTexture("Assets/Achievements/achievement_icon.png");
inline sf::Texture traderIconTexture("Assets/Trader/trader_icon.png");

inline map<string, sf::Texture> achievementTextures;