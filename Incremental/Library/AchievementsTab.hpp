#pragma once

#include "Misc/Includes.hpp"
#include "Achievements/Achievements.hpp"
#include "Achievements/AchievementsList.hpp"
#include "Misc/GlobalTextures.hpp"
#include "Misc/GlobalVariables.hpp"

inline float slideOffset = 0.f;

class AchievementsTab {
public:
    AchievementsTab(sf::Font& fontRef)
        : font(fontRef)
    {}

    void draw(sf::RenderWindow& window, const std::vector<Achievement>& achievements)
    {
        // Slide-in animation
        if (isAnimating)
        {
            float elapsed = slideClock.getElapsedTime().asSeconds();
            float duration = 0.5f;
            float t = min(elapsed / duration, 1.f);
            t = 1.f - pow(1.f - t, 3);
            slideOffset = -1500.f * (1.f - t);
            if (t >= 1.f) isAnimating = false;
        }


        drawBackground(window, slideOffset);
        drawPerkBox(window, slideOffset);
        drawAchievementsGrid(window, achievements, slideOffset);
        drawPagination(window, achievements, slideOffset);
    }

    void handleInput(const sf::Vector2f& mousePos, bool clicked, const std::vector<Achievement>& achievements, sf::RenderWindow& window, float slideOffset)
    {
        if (!clicked) return;

        constexpr int achievementsPerPage = 30;
        constexpr int columns = 6;
        constexpr int rows = achievementsPerPage / columns;
        constexpr float boxWidth = 180.f;
        constexpr float boxHeight = 64.f;
        constexpr float spacingX = 24.f;
        constexpr float spacingY = 16.f;

        float totalWidth = columns * boxWidth + (columns - 1) * spacingX;
        float totalHeight = rows * boxHeight + (rows - 1) * spacingY;

        float offsetX = 10.f;
        float offsetY = -147.f;

        float startX = (window.getSize().x - totalWidth) / 2.f + slideOffset + offsetX;
        float startY = (window.getSize().y - totalHeight) / 2.f + offsetY;

        int totalPages = (achievements.size() + achievementsPerPage - 1) / achievementsPerPage;

        float navY = startY + rows * (boxHeight + spacingY) + 10.f;
        sf::Vector2f navSize = { 80.f, 30.f };
        sf::Vector2f prevPos = { startX, navY };
        sf::Vector2f nextPos = { startX + totalWidth - navSize.x, navY };

        sf::FloatRect prevRect(prevPos, navSize);
        sf::FloatRect nextRect(nextPos, navSize);

        if (prevRect.contains(mousePos) && achievementPage > 0) achievementPage--;
        if (nextRect.contains(mousePos) && (achievementPage + 1) < totalPages) achievementPage++;
    }

    void startAnimation() {
        isAnimating = true;
        slideClock.restart();
    }

private:
    sf::Font& font;
    bool isAnimating = false;
    sf::Clock slideClock;
    int achievementPage = 0;

    void drawBackground(sf::RenderWindow& window, float slideOffset)
    {
        sf::Vector2f bgSize(1215.f, 400.f);
        sf::Vector2f bgPos(
            (window.getSize().x - bgSize.x) / 2.f + 10.f + slideOffset,
            (window.getSize().y - bgSize.y) / 2.f - 147.5f
        );

        sf::RectangleShape bg(bgSize);
        bg.setPosition(bgPos);
        bg.setFillColor(sf::Color(30, 30, 30, 230));
        bg.setOutlineThickness(2.f);
        bg.setOutlineColor(sf::Color(255, 215, 100));
        window.draw(bg);
    }

    void drawPerkBox(sf::RenderWindow& window, float slideOffset)
    {
        sf::Vector2f bgPos(
            (window.getSize().x - 1215.f) / 2.f + 10.f + slideOffset,
            (window.getSize().y - 400.f) / 2.f - 147.5f
        );

        sf::Vector2f perkBoxSize = { 240.f, 260.f };
        sf::Vector2f perkBoxPos = { bgPos.x + 1215.f + 20.f, bgPos.y };

        sf::RectangleShape perkBox(perkBoxSize);
        perkBox.setPosition(perkBoxPos);
        perkBox.setFillColor(sf::Color(40, 40, 40, 230));
        perkBox.setOutlineThickness(2.f);
        perkBox.setOutlineColor(sf::Color(255, 215, 100));
        window.draw(perkBox);

        // Perk header
        sf::Text perkTitle(font);
        perkTitle.setCharacterSize(16);
        perkTitle.setFillColor(sf::Color::Yellow);
        perkTitle.setString("Perk Bonuses");
        perkTitle.setPosition({ perkBoxPos.x + 12.f, perkBoxPos.y + 8.f });
        window.draw(perkTitle);

        // Unlocked achievements count
        int unlockedCount = std::count_if(achievements.begin(), achievements.end(),
            [](const Achievement& a) { return a.isUnlocked; });

        sf::Text countText(font);
        countText.setCharacterSize(12);
        countText.setFillColor(sf::Color(200, 200, 200));
        countText.setString("Achievements Unlocked:\n" + std::to_string(unlockedCount) + "/" + std::to_string(achievements.size()));
        countText.setPosition({ perkBoxPos.x + 12.f, perkBoxPos.y + 32.f });
        window.draw(countText);

        // Individual bonuses
        struct PerkDisplay {
            std::string label;
            float value;
        };

        std::vector<PerkDisplay> perkDisplays = {
            { "\nClick Multiplier", perkManager.clickMultiplier },
            { "\nBPS Multiplier",   perkManager.bpsMultiplier }
        };

        float lineY = perkBoxPos.y + 60.f;
        for (const auto& p : perkDisplays)
        {
            sf::Text line(font);
            line.setCharacterSize(12);
            line.setFillColor(sf::Color::White);
            line.setString(p.label + ":\nx" + std::to_string(p.value).substr(0, 4));
            line.setPosition({ perkBoxPos.x + 12.f, lineY });
            window.draw(line);
            lineY += 30.f;
        }

        // Buff cooldown display
        float buffReduction = perkManager.buffSpawnRateMultiplier - 1.0f;
        int percent = static_cast<int>(std::round(buffReduction * 100.f));
        sf::Text buffLine(font);
        buffLine.setCharacterSize(12);
        buffLine.setFillColor(sf::Color::White);
        buffLine.setString("\nBuff Spawn Rate:\n +" + std::to_string(percent) + "%");
        buffLine.setPosition({ perkBoxPos.x + 12.f, lineY });
        window.draw(buffLine);
    }

    void drawAchievementsGrid(sf::RenderWindow& window, const std::vector<Achievement>& achievements, float slideOffset)
    {
        constexpr int achievementsPerPage = 30;
        constexpr int columns = 6;
        constexpr int rows = achievementsPerPage / columns;
        constexpr float boxWidth = 180.f;
        constexpr float boxHeight = 64.f;
        constexpr float spacingX = 24.f;
        constexpr float spacingY = 16.f;

        float totalWidth = columns * boxWidth + (columns - 1) * spacingX;
        float totalHeight = rows * boxHeight + (rows - 1) * spacingY;

        float offsetX = 10.f;
        float offsetY = -147.f;

        float startX = (window.getSize().x - totalWidth) / 2.f + slideOffset + offsetX;
        float startY = (window.getSize().y - totalHeight) / 2.f + offsetY;

        int totalPages = (achievements.size() + achievementsPerPage - 1) / achievementsPerPage;
        achievementPage = std::clamp(achievementPage, 0, std::max(0, totalPages - 1));

        int startIdx = achievementPage * achievementsPerPage;
        int endIdx = std::min<int>(startIdx + achievementsPerPage, achievements.size());

        for (int i = startIdx; i < endIdx; ++i)
        {
            const Achievement& a = achievements[i];
            int localIndex = i - startIdx;
            int row = localIndex / columns;
            int col = localIndex % columns;

            sf::Vector2f pos = {
                startX + col * (boxWidth + spacingX),
                startY + row * (boxHeight + spacingY)
            };

            sf::RectangleShape box({ boxWidth, boxHeight });
            box.setPosition(pos);
            box.setFillColor(a.isUnlocked ? sf::Color(230, 255, 230) : sf::Color(90, 90, 90));
            box.setOutlineThickness(2.5f);

            // Outline color based on type/unlocked status
            if (a.isUnlocked)
            {
                switch (a.achievementType)
                {
                case AchievementType::TotalBubbles: box.setOutlineColor(sf::Color(100, 180, 255)); break;
                case AchievementType::Clicks: box.setOutlineColor(sf::Color(100, 255, 140)); break;
                case AchievementType::UpgradeCount: box.setOutlineColor(sf::Color(200, 100, 255)); break;
                case AchievementType::SpecificUpgrade: box.setOutlineColor(sf::Color(175, 56, 56)); break;
                case AchievementType::BuffTriggered: box.setOutlineColor(sf::Color(255, 100, 180)); break;
                default: box.setOutlineColor(sf::Color(255, 215, 100)); break;
                }
            }
            else if (!a.isHidden)
                box.setOutlineColor(sf::Color(120, 120, 120));
            else
                box.setOutlineColor(sf::Color(60, 60, 60));

            window.draw(box);

            // Draw sprite icon
            const sf::Texture* texToUse = (a.spriteIcon.has_value() && (!a.isHidden || a.isUnlocked))
                ? &a.spriteIcon->getTexture()
                : &achievementTextures["Locked"];

            if (texToUse && texToUse->getSize().x > 0 && texToUse->getSize().y > 0)
            {
                sf::Sprite icon(*texToUse);
                icon.setScale({ 40.f / texToUse->getSize().x, 40.f / texToUse->getSize().y });
                icon.setPosition({ pos.x + boxWidth - 52.f, pos.y + 12.f }); // aligned right
                if (!a.isUnlocked) icon.setColor(sf::Color(80, 80, 80));
                window.draw(icon);
            }

            // Name
            sf::Text nameText(font);
            nameText.setCharacterSize(12);
            nameText.setString(!a.isHidden || a.isUnlocked ? a.name : "???");
            nameText.setFillColor(sf::Color::Black);
            nameText.setPosition({ pos.x + 6.f, pos.y + 6.f });
            window.draw(nameText);

            // Description
            sf::Text descText(font);
            descText.setCharacterSize(10);
            descText.setString(!a.isHidden || a.isUnlocked ? a.description : "Unlock to reveal");
            descText.setFillColor(sf::Color(30, 30, 30));
            descText.setPosition({ pos.x + 6.f, pos.y + 26.f });
            window.draw(descText);

            // Progress
            if (!a.isUnlocked)
            {
                sf::Text progressText(font);
                progressText.setCharacterSize(10);
                float percent = static_cast<float>(a.progressValue * 100.0);
                progressText.setString(std::to_string(static_cast<int>(percent)) + "%");
                progressText.setFillColor(sf::Color::White);
                progressText.setPosition({ pos.x + boxWidth - 25.f, pos.y + 6.f });
                window.draw(progressText);
            }
        }
    }

    void drawPagination(sf::RenderWindow& window, const std::vector<Achievement>& achievements, float slideOffset)
    {
        constexpr int achievementsPerPage = 30;
        constexpr int columns = 6;
        constexpr int rows = achievementsPerPage / columns;
        constexpr float boxWidth = 180.f;
        constexpr float boxHeight = 64.f;
        constexpr float spacingX = 24.f;
        constexpr float spacingY = 16.f;

        // Same offset as grid
        float offsetX = 10.f;
        float offsetY = -147.f;

        float totalWidth = columns * boxWidth + (columns - 1) * spacingX;
        float totalHeight = rows * boxHeight + (rows - 1) * spacingY;

        float startX = (window.getSize().x - totalWidth) / 2.f + slideOffset + offsetX;
        float startY = (window.getSize().y - totalHeight) / 2.f + offsetY;

        int totalPages = (achievements.size() + achievementsPerPage - 1) / achievementsPerPage;

        // Position directly under the last row
        float navY = startY + rows * (boxHeight + spacingY) + 10.f;

        sf::Vector2f navSize = { 80.f, 30.f };
        sf::Vector2f prevPos = { startX, navY };
        sf::Vector2f nextPos = { startX + totalWidth - navSize.x, navY };

        // Prev button
        sf::RectangleShape prevButton(navSize);
        prevButton.setPosition(prevPos);
        prevButton.setFillColor(achievementPage > 0 ? sf::Color(180, 180, 180) : sf::Color(100, 100, 100));
        window.draw(prevButton);

        sf::Text prevText(font);
        prevText.setCharacterSize(14);
        prevText.setString("Prev");
        prevText.setFillColor(sf::Color::Black);
        prevText.setPosition(prevPos + sf::Vector2f(10.f, 5.f));
        window.draw(prevText);

        // Next button
        sf::RectangleShape nextButton(navSize);
        nextButton.setPosition(nextPos);
        nextButton.setFillColor((achievementPage + 1) < totalPages ? sf::Color(180, 180, 180) : sf::Color(100, 100, 100));
        window.draw(nextButton);

        sf::Text nextText(font);
        nextText.setCharacterSize(14);
        nextText.setString("Next");
        nextText.setFillColor(sf::Color::Black);
        nextText.setPosition(nextPos + sf::Vector2f(10.f, 5.f));
        window.draw(nextText);
    }
};