#pragma once
#include "Misc/Includes.hpp"
#include "Achievements/AchievementsTab.hpp"
#include "Achievements/Achievements.hpp" // or wherever your achievements vector lives

enum class LibrarySubTab {
    Achievements,
    Logbook
};

class Library {
public:
    Library(sf::Font& font, const std::vector<Achievement>& ach)
        : achievementsTab(font), achievements(ach), currentSubTab(LibrarySubTab::Achievements)
    {
    }
        
    void draw(sf::RenderWindow& window) {
        if (currentSubTab == LibrarySubTab::Achievements)
            achievementsTab.draw(window, achievements);
    }

    void handleInput(const sf::Vector2f& mousePos, bool clicked) {
        if (currentSubTab == LibrarySubTab::Achievements)
            achievementsTab.handleInput(mousePos, clicked, achievements);
    }

    void startAchievementsAnimation() {
        achievementsTab.startAnimation();
    }

private:
    AchievementsTab achievementsTab;
    const std::vector<Achievement>& achievements;
    LibrarySubTab currentSubTab;
};
