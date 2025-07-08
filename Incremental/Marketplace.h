#pragma once

#include "Includes.h"
#include "GlobalBuffHandler.h"
#include "GlobalVariables.h"
#include "Popup.h"

inline float marketplaceRestockInterval = 30.0f;

struct marketplaceItemsList {
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
    { "Click Buff Omg?", 0.5f },
    { "Random Buff", 0.005f },
	{ "Chaos Begins.", 0.001f }
};

inline unordered_map<string, long double> dynamicPricingOverrideMap = {
    { "Random Buff", 300.0 },
    { "Chaos Begins.", 1200.0 }
};

inline vector<marketplaceItemsList> marketplaceItems = {
    {
		"Bubble Mania",                         // Item Name
		"+25% BPS for 30s",                     // Item Description
		100000.0,                               // Cost
		100000.0,                               // Base Cost
		30.0f,                                  // Duration
		60.0f,                                  // Cooldown Duration
		0.0f,                                   // Cooldown Remaining
		0, true, true, false, 0.0f,             // Stock Remaining, Unlocked, Visible, Is Active, Remaining
		5, 3,                                   // Max Stock, Stock Remaining
		true, 250000.0,                         // Can Be Restocked Manually, Restock Cost
        0.5f,                                   // Restock Chance
		false, 0.0,                             // Dynamic Pricing, Dynamic Price Multiplier
        []() { globalBuffMultiplier *= 1.25f; },// On Activate
		[]() { globalBuffMultiplier /= 1.25f; } // On Expire
    },
    {
        "Click Buff Omg?",
        "x2 Click Multiplier for 60s",
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
        "Buff Radar",
        "Increases buff spawn rates for 60s",
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
		"Random Buff",
		"Spawns a random buff immediately",
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
        "Chaos Begins.",
        "Spawns a Chaos Bubble immediately",
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
    }
};

inline long double getEffectiveItemCost(const marketplaceItemsList& item, long double currentBps)
{
    if (item.dynamicPricing)
    {
        long double scaledBase = item.dynamicPriceMultiplier * currentBps;
        return round(scaledBase * pow(dynamicInflationRate, item.timesBought));
    }

    return item.cost;
}

inline void tryPurchaseItem(marketplaceItemsList& item, long double& currentBubbles, long double currentBps)
{
    if (!item.unlocked || !item.visible || item.isActive || item.cooldownRemaining > 0.f)
        return;

    if (item.stockRemaining == 0)
        return;

    long double effectiveCost = getEffectiveItemCost(item, currentBps);

    if (currentBubbles < effectiveCost)
        return;

    currentBubbles -= effectiveCost;

    item.timesBought++;

    if (!item.dynamicPricing)
        item.cost = round(item.baseCost * pow(itemInflationRate, item.timesBought));

    item.isActive = true;
    item.remaining = item.duration;
    item.cooldownRemaining = item.cooldownDuration;

    if (item.maxStock != -1)
        item.stockRemaining--;

    if (item.onActivate)
        item.onActivate();
}

inline bool tryManualRestock(marketplaceItemsList& item, long double& currentBubbles)
{
    if (!item.canBeRestockedManually || item.stockRemaining == item.maxStock)
        return false;

    if (currentBubbles < item.restockCost)
        return false;

    currentBubbles -= item.restockCost;
    item.stockRemaining = item.maxStock;
    return true;
}

inline void upgradeMarketplace(float dt)
{
    for (auto& item : marketplaceItems)
    {
        if (item.isActive)
        {
            item.remaining -= dt;
            if (item.remaining <= 0.0f)
            {
                item.isActive = false;
                item.remaining = 0.0f;

                if (item.onExpire) item.onExpire();
            }
        }

        if (item.cooldownRemaining > 0.0f)
        {
            item.cooldownRemaining -= dt;
            if (item.cooldownRemaining < 0.0f)
                item.cooldownRemaining = 0.0f;
        }
    }
}

inline void randomlyRestockMarketplaceItems()
{
    for (auto& item : marketplaceItems)
    {
        if (item.maxStock > 0 && item.stockRemaining < item.maxStock && item.restockChance > 0.f)
        {
            float roll = static_cast<float>(rand()) / RAND_MAX;
            if (roll < item.restockChance)
            {
                item.stockRemaining++;
            }
        }
    }
}

inline void to_json(json& j, const marketplaceItemsList& item)
{
    j = json{
        { "name", item.name },
        { "cost", item.cost },
        { "timesBought", item.timesBought },
        { "unlocked", item.unlocked },
        { "visible", item.visible },
        { "stockRemaining", item.stockRemaining },
        { "canBeRestockedManually", item.canBeRestockedManually },
        { "restockCost", item.restockCost },
        { "restockChance", item.restockChance },
        { "dynamicPricing", item.dynamicPricing },
        { "dynamicPriceMultiplier", item.dynamicPriceMultiplier },
        { "cooldownRemaining", item.cooldownRemaining },
        { "remaining", item.remaining }
    };
}

inline void from_json(const json& j, marketplaceItemsList& item)
{
    j.at("cost").get_to(item.cost);
    j.at("timesBought").get_to(item.timesBought);
    if (j.contains("unlocked")) j.at("unlocked").get_to(item.unlocked);
    if (j.contains("visible"))  j.at("visible").get_to(item.visible);
    if (j.contains("stockRemaining")) j.at("stockRemaining").get_to(item.stockRemaining);
    if (j.contains("canBeRestockedManually")) j.at("canBeRestockedManually").get_to(item.canBeRestockedManually);
    if (j.contains("restockCost")) j.at("restockCost").get_to(item.restockCost);
    if (j.contains("restockChance")) j.at("restockChance").get_to(item.restockChance);
    if (j.contains("dynamicPricing")) j.at("dynamicPricing").get_to(item.dynamicPricing);
    if (j.contains("dynamicPriceMultiplier")) j.at("dynamicPriceMultiplier").get_to(item.dynamicPriceMultiplier);
    if (j.contains("cooldownRemaining")) j.at("cooldownRemaining").get_to(item.cooldownRemaining);
    if (j.contains("remaining")) j.at("remaining").get_to(item.remaining);

    auto it = restockOverrideMap.find(item.name);
    if (it != restockOverrideMap.end()) {
        item.restockChance = it->second;
    }

    auto dpOverride = dynamicPricingOverrideMap.find(item.name);
    if (dpOverride != dynamicPricingOverrideMap.end()) {
        item.dynamicPriceMultiplier = dpOverride->second;
    }
}