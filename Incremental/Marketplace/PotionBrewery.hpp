#pragma once

#include "Misc/Includes.hpp"
#include "Misc/GlobalVariables.hpp"
#include "Buffs/Potions/Potions.hpp"

inline void renderPotionBrewery(
    sf::RenderWindow& window,
    const sf::Font& font,
    const sf::Vector2f& mousePositionF,
    bool justClicked,
    float deltaTime,
    float startX,
    float startY,
    int& breweryPage,
    vector<potionsList>& potions,
    vector<traderItemsList>& traderItems,
    long double& bubbles,
    unordered_map<string, float>& hoverScales
) {
    constexpr int itemsPerPage = 4;
    int totalPages = (potions.size() + itemsPerPage - 1) / itemsPerPage;
    breweryPage = clamp(breweryPage, 0, max(0, totalPages - 1));

    constexpr float itemWidth = 320.f;
    constexpr float itemHeight = 120.f;
    constexpr float padding = 20.f;

    float xOffset = -150.f;
    float yOffset = startY;

    for (int i = 0; i < itemsPerPage; i++) {
        int index = breweryPage * itemsPerPage + i;
        if (index >= (int)potions.size()) break;

        potionsList& potion = potions[index];
        string hoverKey = "brewery_" + potion.potionName;

        sf::Vector2f pos(startX + xOffset, yOffset + i * (itemHeight + padding));

        // Hover + scale
        sf::FloatRect hitbox(pos, { itemWidth, itemHeight });
        bool isHovered = hitbox.contains(mousePositionF);
        float& currentScale = hoverScales[hoverKey];
        float targetScale = isHovered ? 1.05f : 1.0f;
        float lerpSpeed = 8.0f * deltaTime;
        currentScale += (targetScale - currentScale) * lerpSpeed;
        currentScale = clamp(currentScale, 1.0f, 1.1f);

        // Box
        sf::RectangleShape box({ itemWidth, itemHeight });
        box.setPosition(pos);
        box.setScale({ currentScale, currentScale });
        box.setFillColor(sf::Color(240, 240, 255));
        box.setOutlineThickness(2.f);
        box.setOutlineColor(sf::Color(100, 100, 150));
        window.draw(box);

        // Potion name
        sf::Text nameText(font);
        nameText.setCharacterSize(18);
		nameText.setString(potion.potionName);
        nameText.setFillColor(sf::Color::Black);
        nameText.setPosition(pos + sf::Vector2f(10.f, 5.f));
        window.draw(nameText);

        // Description
        sf::Text descText(font);
		descText.setCharacterSize(14);
		descText.setString(potion.potionDescription);
        descText.setFillColor(sf::Color(50, 50, 50));
        descText.setPosition(pos + sf::Vector2f(10.f, 30.f));
        window.draw(descText);

        // Current Level Info
        if (!potion.potionLevel.empty()) {
            const potionUpgrade& lvl = potion.potionLevel[potion.currentLevel];
            string info = "Lvl " + to_string(potion.currentLevel + 1) +
                " | x" + to_string(lvl.potionMultiplier) +
                " | " + to_string((int)lvl.potionDuration) + "s";
            sf::Text infoText(font);
			infoText.setCharacterSize(12);
			infoText.setString(info);
            infoText.setFillColor(sf::Color::Black);
            infoText.setPosition(pos + sf::Vector2f(10.f, 55.f));
            window.draw(infoText);

            if (potion.currentLevel + 1 < (int)potion.potionLevel.size()) {
                const potionUpgrade& next = potion.potionLevel[potion.currentLevel + 1];
                string upgradeInfo = "Next: x" + to_string(next.potionMultiplier) +
                    " | " + to_string((int)next.potionDuration) +
                    "s | Cost: " + to_string((int)next.potionCost);
                sf::Text upgradeText(font);
				upgradeText.setCharacterSize(12);
				upgradeText.setString(upgradeInfo);
                upgradeText.setFillColor(sf::Color(80, 80, 80));
                upgradeText.setPosition(pos + sf::Vector2f(10.f, 75.f));
                window.draw(upgradeText);
            }
        }

        // Brew stock
        string stockInfo = "Stock: " + to_string(potion.potionsAmount);
        sf::Text stockText(font);
		stockText.setCharacterSize(14);
		stockText.setString(stockInfo);
        stockText.setFillColor(sf::Color(100, 100, 150));
        stockText.setPosition(pos + sf::Vector2f(10.f, itemHeight - 25.f));
        window.draw(stockText);

        // Brew Button
        sf::Vector2f brewBtnPos(pos.x + itemWidth - 90.f, pos.y + 10.f);
        sf::RectangleShape brewBtn({ 80.f, 30.f });
        brewBtn.setPosition(brewBtnPos);
        brewBtn.setFillColor(sf::Color(200, 200, 255));
        window.draw(brewBtn);

        sf::Text brewText(font);
		brewText.setCharacterSize(14);
		brewText.setString("Brew");
        brewText.setFillColor(sf::Color::Black);
        brewText.setPosition(brewBtnPos + sf::Vector2f(18.f, 6.f));
        window.draw(brewText);

        if (sf::FloatRect(brewBtnPos, { 80.f, 30.f }).contains(mousePositionF) && justClicked) {
            potion.startBrew(traderItems, bubbles);
        }

        // Upgrade Button
        sf::Vector2f upgradeBtnPos(pos.x + itemWidth - 90.f, pos.y + 50.f);
        sf::RectangleShape upgradeBtn({ 80.f, 30.f });
        upgradeBtn.setPosition(upgradeBtnPos);
        upgradeBtn.setFillColor(sf::Color(220, 255, 220));
        window.draw(upgradeBtn);

        sf::Text upgradeText(font);
		upgradeText.setCharacterSize(14);
		upgradeText.setString("Upgrade");
        upgradeText.setFillColor(sf::Color::Black);
        upgradeText.setPosition(upgradeBtnPos + sf::Vector2f(6.f, 6.f));
        window.draw(upgradeText);

        if (sf::FloatRect(upgradeBtnPos, { 80.f, 30.f }).contains(mousePositionF) && justClicked) {
            potion.upgradePotion((long double&)bubbles);
        }

        // Consume Button
        sf::Vector2f consumeBtnPos(pos.x + itemWidth - 90.f, pos.y + 90.f);
        sf::RectangleShape consumeBtn({ 80.f, 30.f });
        consumeBtn.setPosition(consumeBtnPos);
        consumeBtn.setFillColor(sf::Color(255, 220, 220));
        window.draw(consumeBtn);

        sf::Text consumeText(font);
		consumeText.setCharacterSize(14);
		consumeText.setString("Consume");
        consumeText.setFillColor(sf::Color::Black);
        consumeText.setPosition(consumeBtnPos + sf::Vector2f(2.f, 6.f));
        window.draw(consumeText);

        if (sf::FloatRect(consumeBtnPos, { 80.f, 30.f }).contains(mousePositionF) && justClicked) {
            potion.consumePotion();
        }
    }

    // Pagination controls
    float navY = yOffset + itemsPerPage * (itemHeight + padding);
    sf::Vector2f navSize = { 80.f, 30.f };
    sf::Vector2f prevPos = { startX, navY };
    sf::Vector2f nextPos = { startX + itemWidth - navSize.x, navY };

    // Prev button
    sf::RectangleShape prevButton(navSize);
    prevButton.setPosition(prevPos);
    prevButton.setFillColor(breweryPage > 0 ? sf::Color(180, 180, 180) : sf::Color(100, 100, 100));
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
    nextButton.setFillColor((breweryPage + 1) < totalPages ? sf::Color(180, 180, 180) : sf::Color(100, 100, 100));
    window.draw(nextButton);

    sf::Text nextText(font);
	nextText.setCharacterSize(14);
	nextText.setString("Next");
    nextText.setFillColor(sf::Color::Black);
    nextText.setPosition(nextPos + sf::Vector2f(10.f, 5.f));
    window.draw(nextText);

    // Handle clicks
    if (justClicked) {
        if (sf::FloatRect(prevPos, navSize).contains(mousePositionF) && breweryPage > 0)
            breweryPage--;
        if (sf::FloatRect(nextPos, navSize).contains(mousePositionF) && breweryPage + 1 < totalPages)
            breweryPage++;
    }
}