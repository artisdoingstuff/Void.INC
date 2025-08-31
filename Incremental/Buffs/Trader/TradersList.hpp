#pragma once

#include "Buffs/GlobalBuffHandler.hpp"
#include "Upgrades/Upgrades.hpp"

extern vector<UpgradeItem> upgrades;

inline float traderRestockInterval = 60.0f;

struct traderItemsList {
    string name;
    string description;

    long double cost = 0.0L;
    long double baseCost = 0.0L;

    float duration = 0.0f;
    float cooldownDuration = 60.f;
    float cooldownRemaining = 0.0f;

    int timesBought = 0;

    bool unlocked = true;
    bool visible = true;
    bool isActive = false;

    float remaining = 0.0f;

    int maxStock = -1;
    int stockRemaining = -1;

    bool canBeRestockedManually = false;
    long double restockCost = 0.0L;
    float restockChance = 0.0f;

    bool dynamicPricing = false;
    long double dynamicPriceMultiplier = 0.0L;

    function<void()> onActivate;
    function<void()> onExpire;
};

inline unordered_map<string, float> restockOverrideMap = {
    { "Bubble Mania", 0.5f },
    { "Rapid Clicker", 0.5f },
    { "Bubbly Radar", 0.5f },
    { "Make a Wish", 0.005f },
    { "Chaotic Legend", 0.001f }
};

inline unordered_map<string, long double> dynamicPricingOverrideMap = {
    { "Make a Wish", 300.0 },
    { "Chaotic Legend", 1200.0 }
};

inline vector<traderItemsList> traderItems = {
    {
        "Bubble Mania",                             // Item Name
        "Passively earn 20% more Bubbles for 30s",  // Item Description
        100000.0,                                   // Cost
        100000.0,                                   // Base Cost
        30.0f,                                      // Duration
        60.0f,                                      // Cooldown Duration
        0.0f,                                       // Cooldown Remaining
        0, true, true, false, 0.0f,                 // Times Bought, Unlocked, Visible, Is Active, Remaining
        5, 3,                                       // Max Stock, Stock Remaining
        true, 250000.0,                             // Can Be Restocked Manually, Restock Cost
        0.5f,                                       // Restock Chance
        false, 0.0,                                 // Dynamic Pricing, Dynamic Price Multiplier
        []() { globalBuffMultiplier *= 1.2f; },     // On Activate
        []() { globalBuffMultiplier /= 1.2f; }      // On Expire
    },
    {
        "Rapid Clicker",
        "Clicks are twice as powerful for 60s",
        75000.0,
        75000.0,
        60.0f,
        120.f,
        0.f,
        0, true, true, false, 0.0f,
        5, 3,
        true, 250000.0,
        0.5f,
        false, 0.0,
        []() { clickMultiplier *= 2.0f; },
        []() { clickMultiplier /= 2.0f; }
    },
    {
        "Bubbly Radar",
        "Bubbles spawn twice as fast for 60s",
        150000.0,
        150000.0,
        60.0f,
        180.0f,
        0.f,
        0, true, true, false, 0.0f,
        3, 1,
        false, 0.0,
        0.5f,
        false, 0.0,
        []() { globalBuffSpawnDelayMultiplier *= 0.5f; },
        []() { globalBuffSpawnDelayMultiplier /= 0.5f; }
    },
    {
        "Make a Wish",
        "Summons a random Bubble... or Duck",
        0.0,
        0.0,
        0.0f,
        0.0f,
        0.0f,
        0, true, true, false, 0.0f,
        1, 0,
        false, 0.0,
        0.005f,
        true, 300.0,
        []() {
            sf::Vector2f pos = getGlobalBuffSpawnPosition();
            sf::Vector2f size = { 100.f, 100.f };
            auto result = selectGlobalBuffVariant(pos, size);
            if (result.has_value()) {
                auto& [index, variant, sprite] = result.value();
                spawnGlobalBuff(sprite, index);
            }
        },
        []() {}
    },
    {
        "Chaotic Legend",
        "Summons a Chaos Bubble",
        300000.0,
        300000.0,
        0.0f,
        0.0f,
        0.0f,
        0, true, true, false, 0.0f,
        1, 0,
        false, 0.0,
        0.001f,
        true, 1200.0,
        []() {
            sf::Vector2f spawnPos = getGlobalBuffSpawnPosition();
            spawnBuffByType(GlobalBuffType::Chaos, spawnPos, {100.f, 100.f});
        },
        []() {}
    },
    {
		"Wishing Star",
		"Summons a Shooting Star Bubble",
		300000.0,
		300000.0,
		20.0f,
		0.0f,
		0.0f,
		0, true, true, false, 0.0f,
		1, 0,
		false, 0.0,
        0.0008695f,
		true, 1000.0,
        []() {
            sf::Vector2f spawnPos = getGlobalBuffSpawnPosition();
            spawnBuffByType(GlobalBuffType::ShootingStar, spawnPos, {100.f, 100.f});
            globalBuffMultiplier *= 1.15f;
		},
		[]() { globalBuffMultiplier /= 1.15f; }
    },
    {
        "Nuclear Waste",
        "Summons a Mutated Bubble",
        500000.0,
        500000.0,
        30.0f,
        0.0f,
        0.0f,
        0, true, true, false, 0.0f,
        1, 0,
        false, 0.0,
        0.001f,
        true, 1200.0,
        []() {
            sf::Vector2f spawnPos = getGlobalBuffSpawnPosition();
            spawnBuffByType(GlobalBuffType::Mutated, spawnPos, {100.f, 100.f});
            globalBuffMultiplier /= 1.15f;
        },
        []() { globalBuffMultiplier *= 1.15f; }
    },
    {
		"Marshy Goodness",
        "Earn more passively and through clicks!",
		400000.0,
		400000.0,
		30.0f,
		60.0f,
        0.0f,
		0, true, true, false, 0.0f,
        3, 1,
		false, 0.0,
		0.004f,
		false, 0.0,
        []() {
			globalBuffMultiplier *= 1.3f;
			clickMultiplier *= 1.5f;
        },
        []() {
            globalBuffMultiplier /= 1.3f;
            clickMultiplier /= 1.5f;
        }
    },
    {
		"Bubble Beacon",
        "Bubbles spawn three times as fast for 60s\nAND earn 10% more",
        500000.0,
        500000.0,
        60.0f,
        120.0f,
        0.0f,
        0, true, true, false, 0.0f,
        2, 1,
        false, 0.0,
        0.02f,
        false, 0.0,
        []() {
            globalBuffSpawnDelayMultiplier *= 0.33f;
			globalBuffMultiplier *= 1.1f;
        },
		[]() {
            globalBuffSpawnDelayMultiplier /= 0.33f;
			globalBuffMultiplier /= 1.1f;
        }
    },
    {
		"Midas Bubbles",
		"Spawns a RANDOM Golden Bubble\nAND earn 5% more",
		100000.0,
		100000.0,
        10.0f,
		0.0f,
		0.0f,
		0, true, true, false, 0.0f,
		1, 0,
		false, 0.0,
        0.002f,
		true, 500.0f,
        []() {
            sf::Vector2f spawnPos = getGlobalBuffSpawnPosition();
            sf::Vector2f size = {100.f, 100.f};

            static random_device rd;
            static mt19937 gen(rd());
            std::uniform_real_distribution<float> dist(0.f, 1.f);

            if (dist(gen) < 0.8f) {
                // 80% chance
                spawnBuffByType(GlobalBuffType::Additive, spawnPos, size);
            }
         else {
                // 20% chance
                spawnBuffByType(GlobalBuffType::Multiplicative, spawnPos, size);
            }

            globalBuffMultiplier *= 1.05f;
        },
		[]() { globalBuffMultiplier /= 1.05f; }
    }
};