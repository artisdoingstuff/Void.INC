#pragma once

#include "Includes.h"

// Handles the formatting for bubbles display
string formatDisplayBubbles(long double bubbles, bool forceDecimals = false)
{
    if (bubbles < 1000.0f)
    {
        if (forceDecimals)
        {
            ostringstream bubblesPerSecondStreamFix;
            bubblesPerSecondStreamFix << fixed << setprecision(2) << bubbles;
            return bubblesPerSecondStreamFix.str();
        }
        
        else
        {
            return to_string(static_cast<int>(bubbles));
        }
    }

    const string bubblesSuffixes[] = {
        "", "K", "M", "B", "T", "Qa", "Qi", "Sx", "Sp", "Oc", "No", "Dc",
        "Ud", "Dd", "Td", "Qad", "Qid", "Sxd", "Spd", "Ocd", "Nvd", "Vg",
        "Uvg", "Dvg", "Tvg", "Qavg", "Qivg", "Sxvg", "Spvg", "Ocvg", "Nvvg", "Tg",
        "Utg", "Dtg", "Ttg", "Qattg", "Qitg", "Sxtg", "Sptg", "Octg", "Nvtg", "Qg"
    };

    int logBubbles = log10(bubbles);
    int suffixGroup = logBubbles / 3;

    if (suffixGroup >= static_cast<int>(sizeof(bubblesSuffixes) / sizeof(bubblesSuffixes[0])))
        return "?";

    int mod = logBubbles % 3;
    float mantissa = (bubbles / pow(10, logBubbles)) * pow(10, mod);

    ostringstream displayBubblesStringStream;
    displayBubblesStringStream << fixed << setprecision(2) << mantissa << bubblesSuffixes[suffixGroup];
    return displayBubblesStringStream.str();
}