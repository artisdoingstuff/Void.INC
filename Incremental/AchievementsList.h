#pragma once

#include "Achievements.h"

// Achievements
extern vector<Achievement> achievements;

inline void achievementsList()
{
    // Bubble Achievements
    achievements.push_back({
    "It Begins.",                   // Reference Name
    "Generate 1 Bubble",            // Reference Description
    AchievementType::TotalBubbles,  // Achievement Type
    1                               // Unlock Requirement
        }
    );
    achievements.push_back({ "Bubble Beginner", "Generate 100 Bubbles", AchievementType::TotalBubbles, 100.0 });
    achievements.push_back({ "Bubble Novice", "Generate 1K Bubbles", AchievementType::TotalBubbles, 1000.0 });
    achievements.push_back({ "Bubble Initiate", "Generate 10K Bubbles", AchievementType::TotalBubbles, 10000.0 });
    achievements.push_back({ "Bubble Adept", "Generate 25K Bubbles", AchievementType::TotalBubbles, 25000.0 });
    achievements.push_back({ "Proficient Bubbler", "Generate 50K Bubbles", AchievementType::TotalBubbles, 50000.0 });
    achievements.push_back({ "Skilled Bubbler", "Generate 75K Bubbles", AchievementType::TotalBubbles, 75000.0 });
    achievements.push_back({ "Highly Proficient Bubbler", "Generate 100K Bubbles", AchievementType::TotalBubbles, 100000.0 });
    achievements.push_back({ "Advanced Bubbler", "Generate 250K Bubbles", AchievementType::TotalBubbles, 250000.0 });
    achievements.push_back({ "Bubble Expert", "Generate 500K Bubbles", AchievementType::TotalBubbles, 500000.0 });
    achievements.push_back({ "Bubble Specialist", "Generate 750K Bubbles", AchievementType::TotalBubbles, 750000.0 });
    achievements.push_back({ "Bubble Connoisseur", "Generate 1M Bubbles", AchievementType::TotalBubbles, 1000000.0 });
    achievements.push_back({ "Bubble Apprentice", "Generate 10M Bubbles", AchievementType::TotalBubbles, 10000000.0 });
    achievements.push_back({ "Elite Bubbler", "Generate 25M Bubbles", AchievementType::TotalBubbles, 25000000.0 });
    achievements.push_back({ "Bubble Virtuoso", "Generate 50M Bubbles", AchievementType::TotalBubbles, 50000000.0 });
    achievements.push_back({ "Bubble Master", "Generate 75M Bubbles", AchievementType::TotalBubbles, 75000000.0 });
    achievements.push_back({ "Bubble Grandmaster", "Generate 100M Bubbles", AchievementType::TotalBubbles, 100000000.0 });
    achievements.push_back({ "Paragon Bubbler", "Generate 250M Bubbles", AchievementType::TotalBubbles, 250000000.0 });
    achievements.push_back({ "Exalted Bubbler", "Generate 500M Bubbles", AchievementType::TotalBubbles, 500000000.0 });
    achievements.push_back({ "Mythical Bubbler", "Generate 750M Bubbles", AchievementType::TotalBubbles, 750000000.0 });
    achievements.push_back({ "Supreme Bubbler", "Generate 1B Bubbles", AchievementType::TotalBubbles, 1000000000.0 });
    achievements.push_back({ "Divine Bubbler", "Generate 10B Bubbles", AchievementType::TotalBubbles, 10000000000.0 });
    achievements.push_back({ "Mythos", "Generate 25B Bubbles", AchievementType::TotalBubbles, 2500000000.0 });
    achievements.push_back({ "Aether", "Generate 50B Bubbles", AchievementType::TotalBubbles, 5000000000.0 });
    achievements.push_back({ "Bubble Legend", "Generate 75B Bubbles", AchievementType::TotalBubbles, 7500000000.0 });
    achievements.push_back({ "It only gets harder from here.", "Generate 100B Bubbles", AchievementType::TotalBubbles, 1000000000.0 });
    achievements.push_back({ "Here already? It gets harder.", "Generate 1T Bubbles", AchievementType::TotalBubbles, 10000000000.0 });

    // Clicking Achievements
    achievements.push_back({ "Your First Click!", "Generate 1 Bubble from Clicking", AchievementType::Clicks, 1 });
    achievements.push_back({ "Having fun?", "Generate 100 Bubbles from Clicking", AchievementType::Clicks, 100.0 });
    achievements.push_back({ "Surprised you're still clicking", "Generate 1K Bubbles from Clicking", AchievementType::Clicks, 1000.0 });
    achievements.push_back({ "You must really like clicking", "Generate 10K Bubbles from Clicking", AchievementType::Clicks, 10000.0 });
    achievements.push_back({ "Still clicking?", "Generate 100K Bubbles from Clicking", AchievementType::Clicks, 100000.0 });
    achievements.push_back({ "Soapy Fingers", "Generate 1M Bubbles from Clicking", AchievementType::Clicks, 1000000.0 });
    achievements.push_back({ "Fingers of Steel", "Generate 10M Bubbles from Clicking", AchievementType::Clicks, 10000000.0 });
    achievements.push_back({ "Really?", "Generate 100M Bubbles from Clicking", AchievementType::Clicks, 100000000.0 });
    achievements.push_back({ "Watery Fingers", "Generate 1B Bubbles from Clicking", AchievementType::Clicks, 1000000000.0 });
    achievements.push_back({ "You must be really clean!", "Generate 10B Bubbles from Clicking", AchievementType::Clicks, 10000000000.0 });
    achievements.push_back({ "Autoclicker?", "Generate 100B Bubbles from Clicking", AchievementType::Clicks, 100000000000.0 });

    // Item Achievements
    // Soap Achievements
    achievements.push_back({ "First Bubbly Producer", "Purchase 1 Soap", AchievementType::SpecificUpgrade, 1, "Soap" });
    achievements.push_back({ "Soap Bars", "Purchase 10 Soap", AchievementType::SpecificUpgrade, 10, "Soap" });
    achievements.push_back({ "More Soap!", "Purchase 25 Soap", AchievementType::SpecificUpgrade, 25, "Soap" });
    achievements.push_back({ "Fresh Soap", "Purchase 50 Soap", AchievementType::SpecificUpgrade, 50, "Soap" });
    achievements.push_back({ "Liquid Soap? Where from?", "Purchase 75 Soap", AchievementType::SpecificUpgrade, 75, "Soap" });
    achievements.push_back({ "Frothy Soap", "Purchase 100 Soap", AchievementType::SpecificUpgrade, 100, "Soap" });
    achievements.push_back({ "So...ap", "Purchase 150 Soap", AchievementType::SpecificUpgrade, 150, "Soap" });
    achievements.push_back({ "Soap Supplier", "Purchase 200 Soap", AchievementType::SpecificUpgrade, 200, "Soap" });
    achievements.push_back({ "Soap Empire", "Purchase 250 Soap", AchievementType::SpecificUpgrade, 250, "Soap" });
    achievements.push_back({ "", "Purchase 300 Soap", AchievementType::SpecificUpgrade, 300, "Soap" });
    achievements.push_back({ "", "Purchase 400 Soap", AchievementType::SpecificUpgrade, 400, "Soap" });
    achievements.push_back({ "", "Purchase 500 Soap", AchievementType::SpecificUpgrade, 500, "Soap" });
    achievements.push_back({ "", "Purchase 600 Soap", AchievementType::SpecificUpgrade, 600, "Soap" });
    achievements.push_back({ "", "Purchase 700 Soap", AchievementType::SpecificUpgrade, 700, "Soap" });
    achievements.push_back({ "", "Purchase 800 Soap", AchievementType::SpecificUpgrade, 800, "Soap" });
    achievements.push_back({ "", "Purchase 900 Soap", AchievementType::SpecificUpgrade, 900, "Soap" });
    achievements.push_back({ "", "Purchase 1000 Soap", AchievementType::SpecificUpgrade, 1000, "Soap" });
    achievements.push_back({ "", "Purchase 1500 Soap", AchievementType::SpecificUpgrade, 1500, "Soap" });
    achievements.push_back({ "", "Purchase 2000 Soap", AchievementType::SpecificUpgrade, 2000, "Soap" });
    achievements.push_back({ "", "Purchase 2500 Soap", AchievementType::SpecificUpgrade, 2500, "Soap" });
    achievements.push_back({ "", "Purchase 3000 Soap", AchievementType::SpecificUpgrade, 3000, "Soap" });
    achievements.push_back({ "", "Purchase 4000 Soap", AchievementType::SpecificUpgrade, 4000, "Soap" });
    achievements.push_back({ "True Endgame Soap", "Purchase 5000 Soap", AchievementType::SpecificUpgrade, 5000, "Soap" });

    // Hand Wash Achievements
    achievements.push_back({ "Hand Wash Placeholder 0", "Purchase 1 Hand Wash", AchievementType::SpecificUpgrade, 1, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 1", "Purchase 10 Hand Wash", AchievementType::SpecificUpgrade, 10, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 2", "Purchase 25 Hand Wash", AchievementType::SpecificUpgrade, 25, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 3", "Purchase 50 Hand Wash", AchievementType::SpecificUpgrade, 50, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 4", "Purchase 75 Hand Wash", AchievementType::SpecificUpgrade, 75, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 5", "Purchase 100 Hand Wash", AchievementType::SpecificUpgrade, 100, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 6", "Purchase 150 Hand Wash", AchievementType::SpecificUpgrade, 150, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 7", "Purchase 200 Hand Wash", AchievementType::SpecificUpgrade, 200, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 8", "Purchase 250 Hand Wash", AchievementType::SpecificUpgrade, 250, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 9", "Purchase 300 Hand Wash", AchievementType::SpecificUpgrade, 300, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 10", "Purchase 400 Hand Wash", AchievementType::SpecificUpgrade, 400, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 11", "Purchase 500 Hand Wash", AchievementType::SpecificUpgrade, 500, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 12", "Purchase 600 Hand Wash", AchievementType::SpecificUpgrade, 600, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 13", "Purchase 700 Hand Wash", AchievementType::SpecificUpgrade, 700, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 14", "Purchase 800 Hand Wash", AchievementType::SpecificUpgrade, 800, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 15", "Purchase 900 Hand Wash", AchievementType::SpecificUpgrade, 900, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 16", "Purchase 1000 Hand Wash", AchievementType::SpecificUpgrade, 1000, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 17", "Purchase 1500 Hand Wash", AchievementType::SpecificUpgrade, 1500, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 18", "Purchase 2000 Hand Wash", AchievementType::SpecificUpgrade, 2000, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 19", "Purchase 2500 Hand Wash", AchievementType::SpecificUpgrade, 2500, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 20", "Purchase 3000 Hand Wash", AchievementType::SpecificUpgrade, 3000, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 21", "Purchase 4000 Hand Wash", AchievementType::SpecificUpgrade, 4000, "Hand Wash" });
    achievements.push_back({ "Hand Wash Placeholder 22", "Purchase 5000 Hand Wash", AchievementType::SpecificUpgrade, 5000, "Hand Wash" });

    // Shampoo Achievements
    achievements.push_back({ "Shampoo Placeholder 0", "Purchase 1 Shampoo", AchievementType::SpecificUpgrade, 1, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 1", "Purchase 10 Shampoo", AchievementType::SpecificUpgrade, 10, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 2", "Purchase 25 Shampoo", AchievementType::SpecificUpgrade, 25, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 3", "Purchase 50 Shampoo", AchievementType::SpecificUpgrade, 50, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 4", "Purchase 75 Shampoo", AchievementType::SpecificUpgrade, 75, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 5", "Purchase 100 Shampoo", AchievementType::SpecificUpgrade, 100, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 6", "Purchase 150 Shampoo", AchievementType::SpecificUpgrade, 150, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 7", "Purchase 200 Shampoo", AchievementType::SpecificUpgrade, 200, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 8", "Purchase 250 Shampoo", AchievementType::SpecificUpgrade, 250, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 9", "Purchase 300 Shampoo", AchievementType::SpecificUpgrade, 300, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 10", "Purchase 400 Shampoo", AchievementType::SpecificUpgrade, 400, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 11", "Purchase 500 Shampoo", AchievementType::SpecificUpgrade, 500, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 12", "Purchase 600 Shampoo", AchievementType::SpecificUpgrade, 600, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 13", "Purchase 700 Shampoo", AchievementType::SpecificUpgrade, 700, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 14", "Purchase 800 Shampoo", AchievementType::SpecificUpgrade, 800, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 15", "Purchase 900 Shampoo", AchievementType::SpecificUpgrade, 900, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 16", "Purchase 1000 Shampoo", AchievementType::SpecificUpgrade, 1000, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 17", "Purchase 1500 Shampoo", AchievementType::SpecificUpgrade, 1500, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 18", "Purchase 2000 Shampoo", AchievementType::SpecificUpgrade, 2000, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 19", "Purchase 2500 Shampoo", AchievementType::SpecificUpgrade, 2500, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 20", "Purchase 3000 Shampoo", AchievementType::SpecificUpgrade, 3000, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 21", "Purchase 4000 Shampoo", AchievementType::SpecificUpgrade, 4000, "Shampoo" });
    achievements.push_back({ "Shampoo Placeholder 22", "Purchase 5000 Shampoo", AchievementType::SpecificUpgrade, 5000, "Shampoo" });

    // Shaving Foam Achievements
    achievements.push_back({ "Shaving Foam Placeholder 0", "Purchase 1 Shaving Foam", AchievementType::SpecificUpgrade, 1, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 1", "Purchase 10 Shaving Foam", AchievementType::SpecificUpgrade, 10, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 2", "Purchase 25 Shaving Foam", AchievementType::SpecificUpgrade, 25, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 3", "Purchase 50 Shaving Foam", AchievementType::SpecificUpgrade, 50, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 4", "Purchase 75 Shaving Foam", AchievementType::SpecificUpgrade, 75, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 5", "Purchase 100 Shaving Foam", AchievementType::SpecificUpgrade, 100, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 6", "Purchase 150 Shaving Foam", AchievementType::SpecificUpgrade, 150, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 7", "Purchase 200 Shaving Foam", AchievementType::SpecificUpgrade, 200, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 8", "Purchase 250 Shaving Foam", AchievementType::SpecificUpgrade, 250, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 9", "Purchase 300 Shaving Foam", AchievementType::SpecificUpgrade, 300, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 10", "Purchase 400 Shaving Foam", AchievementType::SpecificUpgrade, 400, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 11", "Purchase 500 Shaving Foam", AchievementType::SpecificUpgrade, 500, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 12", "Purchase 600 Shaving Foam", AchievementType::SpecificUpgrade, 600, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 13", "Purchase 700 Shaving Foam", AchievementType::SpecificUpgrade, 700, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 14", "Purchase 800 Shaving Foam", AchievementType::SpecificUpgrade, 800, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 15", "Purchase 900 Shaving Foam", AchievementType::SpecificUpgrade, 900, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 16", "Purchase 1000 Shaving Foam", AchievementType::SpecificUpgrade, 1000, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 17", "Purchase 1500 Shaving Foam", AchievementType::SpecificUpgrade, 1500, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 18", "Purchase 2000 Shaving Foam", AchievementType::SpecificUpgrade, 2000, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 19", "Purchase 2500 Shaving Foam", AchievementType::SpecificUpgrade, 2500, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 20", "Purchase 3000 Shaving Foam", AchievementType::SpecificUpgrade, 3000, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 21", "Purchase 4000 Shaving Foam", AchievementType::SpecificUpgrade, 4000, "Shaving Foam" });
    achievements.push_back({ "Shaving Foam Placeholder 22", "Purchase 5000 Shaving Foam", AchievementType::SpecificUpgrade, 5000, "Shaving Foam" });

    // Toothpaste Achievements
    achievements.push_back({ "Toothpaste Placeholder 0", "Purchase 1 Toothpaste", AchievementType::SpecificUpgrade, 1, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 1", "Purchase 10 Toothpaste", AchievementType::SpecificUpgrade, 10, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 2", "Purchase 25 Toothpaste", AchievementType::SpecificUpgrade, 25, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 3", "Purchase 50 Toothpaste", AchievementType::SpecificUpgrade, 50, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 4", "Purchase 75 Toothpaste", AchievementType::SpecificUpgrade, 75, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 5", "Purchase 100 Toothpaste", AchievementType::SpecificUpgrade, 100, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 6", "Purchase 150 Toothpaste", AchievementType::SpecificUpgrade, 150, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 7", "Purchase 200 Toothpaste", AchievementType::SpecificUpgrade, 200, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 8", "Purchase 250 Toothpaste", AchievementType::SpecificUpgrade, 250, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 9", "Purchase 300 Toothpaste", AchievementType::SpecificUpgrade, 300, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 10", "Purchase 400 Toothpaste", AchievementType::SpecificUpgrade, 400, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 11", "Purchase 500 Toothpaste", AchievementType::SpecificUpgrade, 500, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 12", "Purchase 600 Toothpaste", AchievementType::SpecificUpgrade, 600, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 13", "Purchase 700 Toothpaste", AchievementType::SpecificUpgrade, 700, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 14", "Purchase 800 Toothpaste", AchievementType::SpecificUpgrade, 800, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 15", "Purchase 900 Toothpaste", AchievementType::SpecificUpgrade, 900, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 16", "Purchase 1000 Toothpaste", AchievementType::SpecificUpgrade, 1000, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 17", "Purchase 1500 Toothpaste", AchievementType::SpecificUpgrade, 1500, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 18", "Purchase 2000 Toothpaste", AchievementType::SpecificUpgrade, 2000, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 19", "Purchase 2500 Toothpaste", AchievementType::SpecificUpgrade, 2500, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 20", "Purchase 3000 Toothpaste", AchievementType::SpecificUpgrade, 3000, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 21", "Purchase 4000 Toothpaste", AchievementType::SpecificUpgrade, 4000, "Toothpaste" });
    achievements.push_back({ "Toothpaste Placeholder 22", "Purchase 5000 Toothpaste", AchievementType::SpecificUpgrade, 5000, "Toothpaste" });

    // Loofah Achievements
    achievements.push_back({ "Loofah Placeholder 0", "Purchase 1 Loofah", AchievementType::SpecificUpgrade, 1, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 1", "Purchase 10 Loofah", AchievementType::SpecificUpgrade, 10, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 2", "Purchase 25 Loofah", AchievementType::SpecificUpgrade, 25, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 3", "Purchase 50 Loofah", AchievementType::SpecificUpgrade, 50, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 4", "Purchase 75 Loofah", AchievementType::SpecificUpgrade, 75, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 5", "Purchase 100 Loofah", AchievementType::SpecificUpgrade, 100, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 6", "Purchase 150 Loofah", AchievementType::SpecificUpgrade, 150, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 7", "Purchase 200 Loofah", AchievementType::SpecificUpgrade, 200, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 8", "Purchase 250 Loofah", AchievementType::SpecificUpgrade, 250, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 9", "Purchase 300 Loofah", AchievementType::SpecificUpgrade, 300, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 10", "Purchase 400 Loofah", AchievementType::SpecificUpgrade, 400, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 11", "Purchase 500 Loofah", AchievementType::SpecificUpgrade, 500, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 12", "Purchase 600 Loofah", AchievementType::SpecificUpgrade, 600, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 13", "Purchase 700 Loofah", AchievementType::SpecificUpgrade, 700, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 14", "Purchase 800 Loofah", AchievementType::SpecificUpgrade, 800, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 15", "Purchase 900 Loofah", AchievementType::SpecificUpgrade, 900, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 16", "Purchase 1000 Loofah", AchievementType::SpecificUpgrade, 1000, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 17", "Purchase 1500 Loofah", AchievementType::SpecificUpgrade, 1500, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 18", "Purchase 2000 Loofah", AchievementType::SpecificUpgrade, 2000, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 19", "Purchase 2500 Loofah", AchievementType::SpecificUpgrade, 2500, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 20", "Purchase 3000 Loofah", AchievementType::SpecificUpgrade, 3000, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 21", "Purchase 4000 Loofah", AchievementType::SpecificUpgrade, 4000, "Loofah" });
    achievements.push_back({ "Loofah Placeholder 22", "Purchase 5000 Loofah", AchievementType::SpecificUpgrade, 5000, "Loofah" });

    // Bubble Bath Achievements
    achievements.push_back({ "Bubble Bath Placeholder 0", "Purchase 1 Bubble Bath", AchievementType::SpecificUpgrade, 1, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 1", "Purchase 10 Bubble Bath", AchievementType::SpecificUpgrade, 10, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 2", "Purchase 25 Bubble Bath", AchievementType::SpecificUpgrade, 25, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 3", "Purchase 50 Bubble Bath", AchievementType::SpecificUpgrade, 50, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 4", "Purchase 75 Bubble Bath", AchievementType::SpecificUpgrade, 75, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 5", "Purchase 100 Bubble Bath", AchievementType::SpecificUpgrade, 100, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 6", "Purchase 150 Bubble Bath", AchievementType::SpecificUpgrade, 150, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 7", "Purchase 200 Bubble Bath", AchievementType::SpecificUpgrade, 200, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 8", "Purchase 250 Bubble Bath", AchievementType::SpecificUpgrade, 250, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 9", "Purchase 300 Bubble Bath", AchievementType::SpecificUpgrade, 300, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 10", "Purchase 400 Bubble Bath", AchievementType::SpecificUpgrade, 400, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 11", "Purchase 500 Bubble Bath", AchievementType::SpecificUpgrade, 500, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 12", "Purchase 600 Bubble Bath", AchievementType::SpecificUpgrade, 600, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 13", "Purchase 700 Bubble Bath", AchievementType::SpecificUpgrade, 700, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 14", "Purchase 800 Bubble Bath", AchievementType::SpecificUpgrade, 800, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 15", "Purchase 900 Bubble Bath", AchievementType::SpecificUpgrade, 900, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 16", "Purchase 1000 Bubble Bath", AchievementType::SpecificUpgrade, 1000, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 17", "Purchase 1500 Bubble Bath", AchievementType::SpecificUpgrade, 1500, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 18", "Purchase 2000 Bubble Bath", AchievementType::SpecificUpgrade, 2000, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 19", "Purchase 2500 Bubble Bath", AchievementType::SpecificUpgrade, 2500, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 20", "Purchase 3000 Bubble Bath", AchievementType::SpecificUpgrade, 3000, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 21", "Purchase 4000 Bubble Bath", AchievementType::SpecificUpgrade, 4000, "Bubble Bath" });
    achievements.push_back({ "Bubble Bath Placeholder 22", "Purchase 5000 Bubble Bath", AchievementType::SpecificUpgrade, 5000, "Bubble Bath" });

    // Bathtub Jet Achievements
    achievements.push_back({ "Bathtub Jet Placeholder 0", "Purchase 1 Bathtub Jet", AchievementType::SpecificUpgrade, 1, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 1", "Purchase 10 Bathtub Jet", AchievementType::SpecificUpgrade, 10, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 2", "Purchase 25 Bathtub Jet", AchievementType::SpecificUpgrade, 25, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 3", "Purchase 50 Bathtub Jet", AchievementType::SpecificUpgrade, 50, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 4", "Purchase 75 Bathtub Jet", AchievementType::SpecificUpgrade, 75, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 5", "Purchase 100 Bathtub Jet", AchievementType::SpecificUpgrade, 100, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 6", "Purchase 150 Bathtub Jet", AchievementType::SpecificUpgrade, 150, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 7", "Purchase 200 Bathtub Jet", AchievementType::SpecificUpgrade, 200, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 8", "Purchase 250 Bathtub Jet", AchievementType::SpecificUpgrade, 250, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 9", "Purchase 300 Bathtub Jet", AchievementType::SpecificUpgrade, 300, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 10", "Purchase 400 Bathtub Jet", AchievementType::SpecificUpgrade, 400, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 11", "Purchase 500 Bathtub Jet", AchievementType::SpecificUpgrade, 500, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 12", "Purchase 600 Bathtub Jet", AchievementType::SpecificUpgrade, 600, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 13", "Purchase 700 Bathtub Jet", AchievementType::SpecificUpgrade, 700, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 14", "Purchase 800 Bathtub Jet", AchievementType::SpecificUpgrade, 800, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 15", "Purchase 900 Bathtub Jet", AchievementType::SpecificUpgrade, 900, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 16", "Purchase 1000 Bathtub Jet", AchievementType::SpecificUpgrade, 1000, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 17", "Purchase 1500 Bathtub Jet", AchievementType::SpecificUpgrade, 1500, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 18", "Purchase 2000 Bathtub Jet", AchievementType::SpecificUpgrade, 2000, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 19", "Purchase 2500 Bathtub Jet", AchievementType::SpecificUpgrade, 2500, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 20", "Purchase 3000 Bathtub Jet", AchievementType::SpecificUpgrade, 3000, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 21", "Purchase 4000 Bathtub Jet", AchievementType::SpecificUpgrade, 4000, "Bathtub Jet" });
    achievements.push_back({ "Bathtub Jet Placeholder 22", "Purchase 5000 Bathtub Jet", AchievementType::SpecificUpgrade, 5000, "Bathtub Jet" });

    // Luxury Spa Achievements
    achievements.push_back({ "Luxury Spa Placeholder 0", "Purchase 1 Luxury Spa", AchievementType::SpecificUpgrade, 1, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 1", "Purchase 10 Luxury Spa", AchievementType::SpecificUpgrade, 10, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 2", "Purchase 25 Luxury Spa", AchievementType::SpecificUpgrade, 25, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 3", "Purchase 50 Luxury Spa", AchievementType::SpecificUpgrade, 50, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 4", "Purchase 75 Luxury Spa", AchievementType::SpecificUpgrade, 75, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 5", "Purchase 100 Luxury Spa", AchievementType::SpecificUpgrade, 100, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 6", "Purchase 150 Luxury Spa", AchievementType::SpecificUpgrade, 150, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 7", "Purchase 200 Luxury Spa", AchievementType::SpecificUpgrade, 200, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 8", "Purchase 250 Luxury Spa", AchievementType::SpecificUpgrade, 250, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 9", "Purchase 300 Luxury Spa", AchievementType::SpecificUpgrade, 300, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 10", "Purchase 400 Luxury Spa", AchievementType::SpecificUpgrade, 400, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 11", "Purchase 500 Luxury Spa", AchievementType::SpecificUpgrade, 500, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 12", "Purchase 600 Luxury Spa", AchievementType::SpecificUpgrade, 600, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 13", "Purchase 700 Luxury Spa", AchievementType::SpecificUpgrade, 700, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 14", "Purchase 800 Luxury Spa", AchievementType::SpecificUpgrade, 800, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 15", "Purchase 900 Luxury Spa", AchievementType::SpecificUpgrade, 900, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 16", "Purchase 1000 Luxury Spa", AchievementType::SpecificUpgrade, 1000, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 17", "Purchase 1500 Luxury Spa", AchievementType::SpecificUpgrade, 1500, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 18", "Purchase 2000 Luxury Spa", AchievementType::SpecificUpgrade, 2000, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 19", "Purchase 2500 Luxury Spa", AchievementType::SpecificUpgrade, 2500, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 20", "Purchase 3000 Luxury Spa", AchievementType::SpecificUpgrade, 3000, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 21", "Purchase 4000 Luxury Spa", AchievementType::SpecificUpgrade, 4000, "Luxury Spa" });
    achievements.push_back({ "Luxury Spa Placeholder 22", "Purchase 5000 Luxury Spa", AchievementType::SpecificUpgrade, 5000, "Luxury Spa" });

    // Foam Pit Achievements
    achievements.push_back({ "Foam Pit Placeholder 0", "Purchase 1 Foam Pit", AchievementType::SpecificUpgrade, 1, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 1", "Purchase 10 Foam Pit", AchievementType::SpecificUpgrade, 10, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 2", "Purchase 25 Foam Pit", AchievementType::SpecificUpgrade, 25, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 3", "Purchase 50 Foam Pit", AchievementType::SpecificUpgrade, 50, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 4", "Purchase 75 Foam Pit", AchievementType::SpecificUpgrade, 75, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 5", "Purchase 100 Foam Pit", AchievementType::SpecificUpgrade, 100, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 6", "Purchase 150 Foam Pit", AchievementType::SpecificUpgrade, 150, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 7", "Purchase 200 Foam Pit", AchievementType::SpecificUpgrade, 200, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 8", "Purchase 250 Foam Pit", AchievementType::SpecificUpgrade, 250, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 9", "Purchase 300 Foam Pit", AchievementType::SpecificUpgrade, 300, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 10", "Purchase 400 Foam Pit", AchievementType::SpecificUpgrade, 400, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 11", "Purchase 500 Foam Pit", AchievementType::SpecificUpgrade, 500, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 12", "Purchase 600 Foam Pit", AchievementType::SpecificUpgrade, 600, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 13", "Purchase 700 Foam Pit", AchievementType::SpecificUpgrade, 700, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 14", "Purchase 800 Foam Pit", AchievementType::SpecificUpgrade, 800, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 15", "Purchase 900 Foam Pit", AchievementType::SpecificUpgrade, 900, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 16", "Purchase 1000 Foam Pit", AchievementType::SpecificUpgrade, 1000, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 17", "Purchase 1500 Foam Pit", AchievementType::SpecificUpgrade, 1500, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 18", "Purchase 2000 Foam Pit", AchievementType::SpecificUpgrade, 2000, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 19", "Purchase 2500 Foam Pit", AchievementType::SpecificUpgrade, 2500, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 20", "Purchase 3000 Foam Pit", AchievementType::SpecificUpgrade, 3000, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 21", "Purchase 4000 Foam Pit", AchievementType::SpecificUpgrade, 4000, "Foam Pit" });
    achievements.push_back({ "Foam Pit Placeholder 22", "Purchase 5000 Foam Pit", AchievementType::SpecificUpgrade, 5000, "Foam Pit" });

    // Foam Party Achievements
    achievements.push_back({ "Foam Party Placeholder 0", "Purchase 1 Foam Party", AchievementType::SpecificUpgrade, 1, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 1", "Purchase 10 Foam Party", AchievementType::SpecificUpgrade, 10, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 2", "Purchase 25 Foam Party", AchievementType::SpecificUpgrade, 25, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 3", "Purchase 50 Foam Party", AchievementType::SpecificUpgrade, 50, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 4", "Purchase 75 Foam Party", AchievementType::SpecificUpgrade, 75, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 5", "Purchase 100 Foam Party", AchievementType::SpecificUpgrade, 100, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 6", "Purchase 150 Foam Party", AchievementType::SpecificUpgrade, 150, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 7", "Purchase 200 Foam Party", AchievementType::SpecificUpgrade, 200, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 8", "Purchase 250 Foam Party", AchievementType::SpecificUpgrade, 250, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 9", "Purchase 300 Foam Party", AchievementType::SpecificUpgrade, 300, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 10", "Purchase 400 Foam Party", AchievementType::SpecificUpgrade, 400, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 11", "Purchase 500 Foam Party", AchievementType::SpecificUpgrade, 500, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 12", "Purchase 600 Foam Party", AchievementType::SpecificUpgrade, 600, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 13", "Purchase 700 Foam Party", AchievementType::SpecificUpgrade, 700, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 14", "Purchase 800 Foam Party", AchievementType::SpecificUpgrade, 800, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 15", "Purchase 900 Foam Party", AchievementType::SpecificUpgrade, 900, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 16", "Purchase 1000 Foam Party", AchievementType::SpecificUpgrade, 1000, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 17", "Purchase 1500 Foam Party", AchievementType::SpecificUpgrade, 1500, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 18", "Purchase 2000 Foam Party", AchievementType::SpecificUpgrade, 2000, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 19", "Purchase 2500 Foam Party", AchievementType::SpecificUpgrade, 2500, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 20", "Purchase 3000 Foam Party", AchievementType::SpecificUpgrade, 3000, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 21", "Purchase 4000 Foam Party", AchievementType::SpecificUpgrade, 4000, "Foam Party" });
    achievements.push_back({ "Foam Party Placeholder 22", "Purchase 5000 Foam Party", AchievementType::SpecificUpgrade, 5000, "Foam Party" });

    // Sudsy Soap Achievements
    achievements.push_back({ "Sudsy Soap Placeholder 0", "Purchase 1 Sudsy Soap", AchievementType::SpecificUpgrade, 1, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 1", "Purchase 10 Sudsy Soap", AchievementType::SpecificUpgrade, 10, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 2", "Purchase 25 Sudsy Soap", AchievementType::SpecificUpgrade, 25, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 3", "Purchase 50 Sudsy Soap", AchievementType::SpecificUpgrade, 50, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 4", "Purchase 75 Sudsy Soap", AchievementType::SpecificUpgrade, 75, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 5", "Purchase 100 Sudsy Soap", AchievementType::SpecificUpgrade, 100, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 6", "Purchase 150 Sudsy Soap", AchievementType::SpecificUpgrade, 150, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 7", "Purchase 200 Sudsy Soap", AchievementType::SpecificUpgrade, 200, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 8", "Purchase 250 Sudsy Soap", AchievementType::SpecificUpgrade, 250, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 9", "Purchase 300 Sudsy Soap", AchievementType::SpecificUpgrade, 300, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 10", "Purchase 400 Sudsy Soap", AchievementType::SpecificUpgrade, 400, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 11", "Purchase 500 Sudsy Soap", AchievementType::SpecificUpgrade, 500, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 12", "Purchase 600 Sudsy Soap", AchievementType::SpecificUpgrade, 600, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 13", "Purchase 700 Sudsy Soap", AchievementType::SpecificUpgrade, 700, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 14", "Purchase 800 Sudsy Soap", AchievementType::SpecificUpgrade, 800, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 15", "Purchase 900 Sudsy Soap", AchievementType::SpecificUpgrade, 900, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 16", "Purchase 1000 Sudsy Soap", AchievementType::SpecificUpgrade, 1000, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 17", "Purchase 1500 Sudsy Soap", AchievementType::SpecificUpgrade, 1500, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 18", "Purchase 2000 Sudsy Soap", AchievementType::SpecificUpgrade, 2000, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 19", "Purchase 2500 Sudsy Soap", AchievementType::SpecificUpgrade, 2500, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 20", "Purchase 3000 Sudsy Soap", AchievementType::SpecificUpgrade, 3000, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 21", "Purchase 4000 Sudsy Soap", AchievementType::SpecificUpgrade, 4000, "Sudsy Soap" });
    achievements.push_back({ "Sudsy Soap Placeholder 22", "Purchase 5000 Sudsy Soap", AchievementType::SpecificUpgrade, 5000, "Sudsy Soap" });

    // Bubble Machine Achievements
    achievements.push_back({ "Bubble Machine Placeholder 0", "Purchase 1 Bubble Machine", AchievementType::SpecificUpgrade, 1, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 1", "Purchase 10 Bubble Machine", AchievementType::SpecificUpgrade, 10, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 2", "Purchase 25 Bubble Machine", AchievementType::SpecificUpgrade, 25, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 3", "Purchase 50 Bubble Machine", AchievementType::SpecificUpgrade, 50, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 4", "Purchase 75 Bubble Machine", AchievementType::SpecificUpgrade, 75, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 5", "Purchase 100 Bubble Machine", AchievementType::SpecificUpgrade, 100, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 6", "Purchase 150 Bubble Machine", AchievementType::SpecificUpgrade, 150, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 7", "Purchase 200 Bubble Machine", AchievementType::SpecificUpgrade, 200, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 8", "Purchase 250 Bubble Machine", AchievementType::SpecificUpgrade, 250, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 9", "Purchase 300 Bubble Machine", AchievementType::SpecificUpgrade, 300, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 10", "Purchase 400 Bubble Machine", AchievementType::SpecificUpgrade, 400, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 11", "Purchase 500 Bubble Machine", AchievementType::SpecificUpgrade, 500, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 12", "Purchase 600 Bubble Machine", AchievementType::SpecificUpgrade, 600, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 13", "Purchase 700 Bubble Machine", AchievementType::SpecificUpgrade, 700, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 14", "Purchase 800 Bubble Machine", AchievementType::SpecificUpgrade, 800, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 15", "Purchase 900 Bubble Machine", AchievementType::SpecificUpgrade, 900, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 16", "Purchase 1000 Bubble Machine", AchievementType::SpecificUpgrade, 1000, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 17", "Purchase 1500 Bubble Machine", AchievementType::SpecificUpgrade, 1500, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 18", "Purchase 2000 Bubble Machine", AchievementType::SpecificUpgrade, 2000, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 19", "Purchase 2500 Bubble Machine", AchievementType::SpecificUpgrade, 2500, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 20", "Purchase 3000 Bubble Machine", AchievementType::SpecificUpgrade, 3000, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 21", "Purchase 4000 Bubble Machine", AchievementType::SpecificUpgrade, 4000, "Bubble Machine" });
    achievements.push_back({ "Bubble Machine Placeholder 22", "Purchase 5000 Bubble Machine", AchievementType::SpecificUpgrade, 5000, "Bubble Machine" });

    // Bubbly Pool Achievements
    achievements.push_back({ "Bubbly Pool Placeholder 0", "Purchase 1 Bubbly Pool", AchievementType::SpecificUpgrade, 1, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 1", "Purchase 10 Bubbly Pool", AchievementType::SpecificUpgrade, 10, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 2", "Purchase 25 Bubbly Pool", AchievementType::SpecificUpgrade, 25, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 3", "Purchase 50 Bubbly Pool", AchievementType::SpecificUpgrade, 50, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 4", "Purchase 75 Bubbly Pool", AchievementType::SpecificUpgrade, 75, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 5", "Purchase 100 Bubbly Pool", AchievementType::SpecificUpgrade, 100, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 6", "Purchase 150 Bubbly Pool", AchievementType::SpecificUpgrade, 150, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 7", "Purchase 200 Bubbly Pool", AchievementType::SpecificUpgrade, 200, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 8", "Purchase 250 Bubbly Pool", AchievementType::SpecificUpgrade, 250, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 9", "Purchase 300 Bubbly Pool", AchievementType::SpecificUpgrade, 300, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 10", "Purchase 400 Bubbly Pool", AchievementType::SpecificUpgrade, 400, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 11", "Purchase 500 Bubbly Pool", AchievementType::SpecificUpgrade, 500, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 12", "Purchase 600 Bubbly Pool", AchievementType::SpecificUpgrade, 600, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 13", "Purchase 700 Bubbly Pool", AchievementType::SpecificUpgrade, 700, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 14", "Purchase 800 Bubbly Pool", AchievementType::SpecificUpgrade, 800, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 15", "Purchase 900 Bubbly Pool", AchievementType::SpecificUpgrade, 900, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 16", "Purchase 1000 Bubbly Pool", AchievementType::SpecificUpgrade, 1000, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 17", "Purchase 1500 Bubbly Pool", AchievementType::SpecificUpgrade, 1500, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 18", "Purchase 2000 Bubbly Pool", AchievementType::SpecificUpgrade, 2000, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 19", "Purchase 2500 Bubbly Pool", AchievementType::SpecificUpgrade, 2500, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 20", "Purchase 3000 Bubbly Pool", AchievementType::SpecificUpgrade, 3000, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 21", "Purchase 4000 Bubbly Pool", AchievementType::SpecificUpgrade, 4000, "Bubbly Pool" });
    achievements.push_back({ "Bubbly Pool Placeholder 22", "Purchase 5000 Bubbly Pool", AchievementType::SpecificUpgrade, 5000, "Bubbly Pool" });

    // Sparkling Water Achievements
    achievements.push_back({ "Sparkling Water Placeholder 0", "Purchase 1 Sparkling Water", AchievementType::SpecificUpgrade, 1, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 1", "Purchase 10 Sparkling Water", AchievementType::SpecificUpgrade, 10, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 2", "Purchase 25 Sparkling Water", AchievementType::SpecificUpgrade, 25, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 3", "Purchase 50 Sparkling Water", AchievementType::SpecificUpgrade, 50, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 4", "Purchase 75 Sparkling Water", AchievementType::SpecificUpgrade, 75, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 5", "Purchase 100 Sparkling Water", AchievementType::SpecificUpgrade, 100, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 6", "Purchase 150 Sparkling Water", AchievementType::SpecificUpgrade, 150, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 7", "Purchase 200 Sparkling Water", AchievementType::SpecificUpgrade, 200, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 8", "Purchase 250 Sparkling Water", AchievementType::SpecificUpgrade, 250, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 9", "Purchase 300 Sparkling Water", AchievementType::SpecificUpgrade, 300, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 10", "Purchase 400 Sparkling Water", AchievementType::SpecificUpgrade, 400, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 11", "Purchase 500 Sparkling Water", AchievementType::SpecificUpgrade, 500, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 12", "Purchase 600 Sparkling Water", AchievementType::SpecificUpgrade, 600, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 13", "Purchase 700 Sparkling Water", AchievementType::SpecificUpgrade, 700, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 14", "Purchase 800 Sparkling Water", AchievementType::SpecificUpgrade, 800, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 15", "Purchase 900 Sparkling Water", AchievementType::SpecificUpgrade, 900, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 16", "Purchase 1000 Sparkling Water", AchievementType::SpecificUpgrade, 1000, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 17", "Purchase 1500 Sparkling Water", AchievementType::SpecificUpgrade, 1500, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 18", "Purchase 2000 Sparkling Water", AchievementType::SpecificUpgrade, 2000, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 19", "Purchase 2500 Sparkling Water", AchievementType::SpecificUpgrade, 2500, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 20", "Purchase 3000 Sparkling Water", AchievementType::SpecificUpgrade, 3000, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 21", "Purchase 4000 Sparkling Water", AchievementType::SpecificUpgrade, 4000, "Sparkling Water" });
    achievements.push_back({ "Sparkling Water Placeholder 22", "Purchase 5000 Sparkling Water", AchievementType::SpecificUpgrade, 5000, "Sparkling Water" });

    // Carbonated Soda Achievements
    achievements.push_back({ "Carbonated Soda Placeholder 0", "Purchase 1 Carbonated Soda", AchievementType::SpecificUpgrade, 1, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 1", "Purchase 10 Carbonated Soda", AchievementType::SpecificUpgrade, 10, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 2", "Purchase 25 Carbonated Soda", AchievementType::SpecificUpgrade, 25, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 3", "Purchase 50 Carbonated Soda", AchievementType::SpecificUpgrade, 50, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 4", "Purchase 75 Carbonated Soda", AchievementType::SpecificUpgrade, 75, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 5", "Purchase 100 Carbonated Soda", AchievementType::SpecificUpgrade, 100, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 6", "Purchase 150 Carbonated Soda", AchievementType::SpecificUpgrade, 150, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 7", "Purchase 200 Carbonated Soda", AchievementType::SpecificUpgrade, 200, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 8", "Purchase 250 Carbonated Soda", AchievementType::SpecificUpgrade, 250, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 9", "Purchase 300 Carbonated Soda", AchievementType::SpecificUpgrade, 300, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 10", "Purchase 400 Carbonated Soda", AchievementType::SpecificUpgrade, 400, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 11", "Purchase 500 Carbonated Soda", AchievementType::SpecificUpgrade, 500, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 12", "Purchase 600 Carbonated Soda", AchievementType::SpecificUpgrade, 600, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 13", "Purchase 700 Carbonated Soda", AchievementType::SpecificUpgrade, 700, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 14", "Purchase 800 Carbonated Soda", AchievementType::SpecificUpgrade, 800, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 15", "Purchase 900 Carbonated Soda", AchievementType::SpecificUpgrade, 900, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 16", "Purchase 1000 Carbonated Soda", AchievementType::SpecificUpgrade, 1000, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 17", "Purchase 1500 Carbonated Soda", AchievementType::SpecificUpgrade, 1500, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 18", "Purchase 2000 Carbonated Soda", AchievementType::SpecificUpgrade, 2000, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 19", "Purchase 2500 Carbonated Soda", AchievementType::SpecificUpgrade, 2500, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 20", "Purchase 3000 Carbonated Soda", AchievementType::SpecificUpgrade, 3000, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 21", "Purchase 4000 Carbonated Soda", AchievementType::SpecificUpgrade, 4000, "Carbonated Soda" });
    achievements.push_back({ "Carbonated Soda Placeholder 22", "Purchase 5000 Carbonated Soda", AchievementType::SpecificUpgrade, 5000, "Carbonated Soda" });

    // Bath Bombs Achievements
    achievements.push_back({ "Bath Bombs Placeholder 0", "Purchase 1 Bath Bombs", AchievementType::SpecificUpgrade, 1, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 1", "Purchase 10 Bath Bombs", AchievementType::SpecificUpgrade, 10, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 2", "Purchase 25 Bath Bombs", AchievementType::SpecificUpgrade, 25, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 3", "Purchase 50 Bath Bombs", AchievementType::SpecificUpgrade, 50, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 4", "Purchase 75 Bath Bombs", AchievementType::SpecificUpgrade, 75, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 5", "Purchase 100 Bath Bombs", AchievementType::SpecificUpgrade, 100, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 6", "Purchase 150 Bath Bombs", AchievementType::SpecificUpgrade, 150, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 7", "Purchase 200 Bath Bombs", AchievementType::SpecificUpgrade, 200, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 8", "Purchase 250 Bath Bombs", AchievementType::SpecificUpgrade, 250, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 9", "Purchase 300 Bath Bombs", AchievementType::SpecificUpgrade, 300, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 10", "Purchase 400 Bath Bombs", AchievementType::SpecificUpgrade, 400, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 11", "Purchase 500 Bath Bombs", AchievementType::SpecificUpgrade, 500, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 12", "Purchase 600 Bath Bombs", AchievementType::SpecificUpgrade, 600, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 13", "Purchase 700 Bath Bombs", AchievementType::SpecificUpgrade, 700, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 14", "Purchase 800 Bath Bombs", AchievementType::SpecificUpgrade, 800, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 15", "Purchase 900 Bath Bombs", AchievementType::SpecificUpgrade, 900, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 16", "Purchase 1000 Bath Bombs", AchievementType::SpecificUpgrade, 1000, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 17", "Purchase 1500 Bath Bombs", AchievementType::SpecificUpgrade, 1500, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 18", "Purchase 2000 Bath Bombs", AchievementType::SpecificUpgrade, 2000, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 19", "Purchase 2500 Bath Bombs", AchievementType::SpecificUpgrade, 2500, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 20", "Purchase 3000 Bath Bombs", AchievementType::SpecificUpgrade, 3000, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 21", "Purchase 4000 Bath Bombs", AchievementType::SpecificUpgrade, 4000, "Bath Bombs" });
    achievements.push_back({ "Bath Bombs Placeholder 22", "Purchase 5000 Bath Bombs", AchievementType::SpecificUpgrade, 5000, "Bath Bombs" });

    // Bubble Wand Achievements
    achievements.push_back({ "Bubble Wand Placeholder 0", "Purchase 1 Bubble Wand", AchievementType::SpecificUpgrade, 1, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 1", "Purchase 10 Bubble Wand", AchievementType::SpecificUpgrade, 10, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 2", "Purchase 25 Bubble Wand", AchievementType::SpecificUpgrade, 25, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 3", "Purchase 50 Bubble Wand", AchievementType::SpecificUpgrade, 50, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 4", "Purchase 75 Bubble Wand", AchievementType::SpecificUpgrade, 75, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 5", "Purchase 100 Bubble Wand", AchievementType::SpecificUpgrade, 100, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 6", "Purchase 150 Bubble Wand", AchievementType::SpecificUpgrade, 150, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 7", "Purchase 200 Bubble Wand", AchievementType::SpecificUpgrade, 200, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 8", "Purchase 250 Bubble Wand", AchievementType::SpecificUpgrade, 250, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 9", "Purchase 300 Bubble Wand", AchievementType::SpecificUpgrade, 300, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 10", "Purchase 400 Bubble Wand", AchievementType::SpecificUpgrade, 400, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 11", "Purchase 500 Bubble Wand", AchievementType::SpecificUpgrade, 500, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 12", "Purchase 600 Bubble Wand", AchievementType::SpecificUpgrade, 600, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 13", "Purchase 700 Bubble Wand", AchievementType::SpecificUpgrade, 700, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 14", "Purchase 800 Bubble Wand", AchievementType::SpecificUpgrade, 800, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 15", "Purchase 900 Bubble Wand", AchievementType::SpecificUpgrade, 900, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 16", "Purchase 1000 Bubble Wand", AchievementType::SpecificUpgrade, 1000, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 17", "Purchase 1500 Bubble Wand", AchievementType::SpecificUpgrade, 1500, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 18", "Purchase 2000 Bubble Wand", AchievementType::SpecificUpgrade, 2000, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 19", "Purchase 2500 Bubble Wand", AchievementType::SpecificUpgrade, 2500, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 20", "Purchase 3000 Bubble Wand", AchievementType::SpecificUpgrade, 3000, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 21", "Purchase 4000 Bubble Wand", AchievementType::SpecificUpgrade, 4000, "Bubble Wand" });
    achievements.push_back({ "Bubble Wand Placeholder 22", "Purchase 5000 Bubble Wand", AchievementType::SpecificUpgrade, 5000, "Bubble Wand" });
}