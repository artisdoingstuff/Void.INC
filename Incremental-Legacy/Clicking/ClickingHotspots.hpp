#pragma once

#include "Misc/Includes.hpp"
#include "Misc/GlobalFunctions.hpp"
#include "Misc/GlobalVariables.hpp"

inline void initializeHotspotSystem()
{
    hotspotSpawnInterval = randomFloat(20.f, 30.f);
    hotspotClock.restart();
    hotspotActive = false;
}

inline void updateHotspotSystem(sf::Vector2f& clickAreaSize, sf::Vector2f& clickAreaPosition)
{
    float elapsed = hotspotClock.getElapsedTime().asSeconds();

    if (!hotspotActive)
    {
        if (elapsed >= hotspotSpawnInterval)
        {
            float hotspotWidth = 40.f;
            float hotspotHeight = 40.f;

            float marginX = clickAreaSize.x - hotspotWidth;
            float marginY = clickAreaSize.y - hotspotHeight;

            float randX = clickAreaPosition.x + randomFloat(0.f, marginX);
            float randY = clickAreaPosition.y + randomFloat(0.f, marginY);

            currentHotspot = sf::FloatRect({ randX, randY }, { hotspotWidth, hotspotHeight });

            hotspotDuration = randomFloat(5.f, 15.f);
            hotspotActive = true;

            hotspotClock.restart();
        }
    }
    else
    {
        if (elapsed >= hotspotDuration)
        {
            hotspotActive = false;
            hotspotSpawnInterval = randomFloat(20.f, 30.f);
            hotspotClock.restart();
        }
    }
}