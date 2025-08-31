#pragma once

#include "Misc/Includes.hpp"

// Offline bubbles
void offlineBubbles(time_t timestamp, long double& bubbles, long double& allTimeBubbles, long double bubblesPerSecond)
{
    time_t currentTime = time(nullptr);
    time_t elapsedTime = currentTime - timestamp;

    if (elapsedTime > 0)
    {
        long double offlineBubbles = (elapsedTime * bubblesPerSecond) * 0.5;
        bubbles += offlineBubbles;
        allTimeBubbles += offlineBubbles;
        cout << "Offline bubbles: " << offlineBubbles << " bubbles" << endl;
    }
}