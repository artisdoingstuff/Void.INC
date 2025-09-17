#pragma once

#include "Misc/Includes.hpp"
#include "Misc/GlobalVariables.hpp"
#include "Potions.hpp"

inline vector<potionsList> potions = {
    {
        "Bubbly Elixir",
        "Boosts your BPS for a limited time",
        0,                                      // currentLevel
        false, false, false,                    // unlocked, brewed, active
        0,                                      // potionsAmount
        0.f,                                    // potionRemainingTimer
        60.f,                                   // brewTime
        { {"Bubble Mania", 1} },                // Requirements (Based on trader)
        {
            {1.10f, 60.f, 500000.f},            // Level 0: +10%, 1m, 500k
            {1.15f, 60.f, 1000000.f},           // Level 1: +15%, 1m, 1M
            {1.15f, 90.f, 2000000.f},           // Level 2: +15%, 1.5m, 2M
            {1.20f, 90.f, 4000000.f},           // Level 3: +20%, 1.5m, 4M
        },
        [](const potionUpgrade& upgrade) {
            globalBuffMultiplier *= upgrade.potionMultiplier;
        },
        [](const potionUpgrade& upgrade) {
            globalBuffMultiplier /= upgrade.potionMultiplier;
        }
    }
};
