#pragma once

#include "Misc/Includes.hpp"
#include "Buffs/GlobalBuffHandler.hpp"
#include "Misc/GlobalVariables.hpp"
#include "Misc/Popup.hpp"
#include "Buffs/Trader/TradersList.hpp"

inline long double getEffectiveItemCost(const traderItemsList& item, long double currentBps)
{
    if (item.dynamicPricing)
    {
        long double scaledBase = item.dynamicPriceMultiplier * currentBps;
        return round(scaledBase * pow(dynamicInflationRate, item.timesBought));
    }

    return item.cost;
}

inline void tryPurchaseItem(traderItemsList& item, long double& currentBubbles, long double currentBps)
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

inline bool tryManualRestock(traderItemsList& item, long double& currentBubbles)
{
    if (!item.canBeRestockedManually || item.stockRemaining == item.maxStock)
        return false;

    if (currentBubbles < item.restockCost)
        return false;

    currentBubbles -= item.restockCost;
    item.stockRemaining = item.maxStock;
    return true;
}

inline void upgradeTrader(float dt)
{
    for (auto& item : traderItems)
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

inline void randomlyRestockTraderItems()
{
    for (auto& item : traderItems)
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

inline void to_json(json& j, const traderItemsList& item)
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

inline void from_json(const json& j, traderItemsList& item)
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