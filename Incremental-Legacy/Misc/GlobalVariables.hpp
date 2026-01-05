#pragma once

inline float globalBuffSpawnDelayMultiplier = 1.0f;
inline float globalCostMultiplier = 1.0f;
inline float globalBuffMultiplier = 1.0f;

inline const long double shopInflationMultiplier = 1.15L;
inline const long double itemInflationRate = 1.05L;
inline const long double dynamicInflationRate = 1.01L;

inline long double prestigePoints = 0.0L;

inline long double bubbles = 0.0L;
inline long double allTimeBubbles = 0.0L;
inline long double allTimeBubblesPerClick = 0.0L;

inline long double bubblesPerSecond = 0.0L;

inline long double baseBubblesPerClick = 1.0L;
inline long double clickMultiplier = 1.0L;

inline long double totalUpgradeCount = 0.0L;

inline long double displayBubbles = 0.0L;

inline long double buffCounter = 0.0L;
inline long double duckCounter = 0.0L;

inline namespace UIConstants
{
    constexpr float TabWidth = 150.0f;
    constexpr float TabHeight = 40.0f;
    constexpr float TabSpacing = 10.0f;
    constexpr float TabRightMargin = 40.0f;
    constexpr float TabTopOffset = 50.0f;

    constexpr float StartYOffset = UIConstants::TabTopOffset + UIConstants::TabHeight + 20.0f;
}

// Precision Clicks (Hotspot) variables
inline sf::Clock hotspotClock;
inline float hotspotSpawnInterval = 0.0f;
inline float hotspotDuration = 0.0f;

inline bool hotspotActive = false;
inline sf::FloatRect currentHotspot;