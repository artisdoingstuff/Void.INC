#pragma once

#include "Includes.hpp"
#include "GlobalVariables.hpp"
#include "Trader.hpp"
#include "Popup.hpp"
#include "UpgradesList.hpp"

inline int ascensionCount = 0;
inline long double ascensionMultiplier = 1.0L;

inline long double ascensionTokenMultiplier = 1.0L;
inline int ascensionTokens = 0;

inline constexpr long double baseAscensionCost = 1e14L; // 100t
inline constexpr long double ascensionCostScaling = 1.05L;

inline void resetGameStateOnAscension()
{
    bubbles = 0.0L;
    allTimeBubbles = 0.0L;
    allTimeBubblesPerClick = 0.0L;
    bubblesPerSecond = 0.0L;

    for (auto& upgrade : upgrades) {
        upgrade.count = 0;
    }

    for (auto& item : traderItems) {
        item.timesBought = 0;
        item.stockRemaining = item.maxStock;
        item.cooldownRemaining = 0.0f;
        item.isActive = false;
        item.remaining = 0.0f;
        item.cost = item.baseCost;
    }

    globalBuffMultiplier = 1.0f;
}


inline long double getTotalAscensionCost(int currentAscensions, int ascensionsToBuy)
{
    long double totalCost = 0.0L;
    for (int i = 0; i < ascensionsToBuy; ++i)
    {
        totalCost += baseAscensionCost * pow(ascensionCostScaling, currentAscensions + i);
    }
    return totalCost;
}

inline int getMaxAffordableAscensions(int currentAscensions, long double availableBubbles)
{
    int count = 0;
    long double totalCost = 0.0L;

    while (true)
    {
        long double nextCost = baseAscensionCost * pow(ascensionCostScaling, currentAscensions + count);
        if (totalCost + nextCost > availableBubbles)
            break;
        totalCost += nextCost;
        ++count;
    }

    return count;
}

inline void doAscension(int ascensionsToGain, long double& allTimeBubbles, long double& currentBubbles, queue<PopupStruct>& popupQueue)
{
    long double cost = getTotalAscensionCost(ascensionCount, ascensionsToGain);
    if (allTimeBubbles < cost) return;

    allTimeBubbles -= cost;
    ascensionCount += ascensionsToGain;

    ascensionMultiplier = 1.0L + ascensionCount * 0.05L;
    ascensionTokens += static_cast<int>(ascensionsToGain * ascensionTokenMultiplier);

    resetGameStateOnAscension();

    popupQueue.push({ "Ascension Complete!", 0.f, 3.0f });
}

inline void saveAscension(json& j)
{
    j["ascensionCount"] = ascensionCount;
    j["ascensionTokens"] = ascensionTokens;
    j["ascensionMultiplier"] = ascensionMultiplier;
}

inline void loadAscension(const json& j)
{
    if (j.contains("ascensionCount")) j["ascensionCount"].get_to(ascensionCount);
    if (j.contains("ascensionTokens")) j["ascensionTokens"].get_to(ascensionTokens);
    if (j.contains("ascensionMultiplier")) j["ascensionMultiplier"].get_to(ascensionMultiplier);
}