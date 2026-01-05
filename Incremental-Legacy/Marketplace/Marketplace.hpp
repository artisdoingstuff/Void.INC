#pragma once

#include "Misc/Includes.hpp"
#include "PotionBrewery.hpp"
#include "Trader.hpp"

enum class MarketplaceTab {
    Trader,
    Brewery
};

MarketplaceTab currentMarketplaceTab = MarketplaceTab::Trader;

inline void renderMarketplace(
    sf::RenderWindow& window,
    const sf::Font& font,
    const sf::Vector2f& mousePositionF,
    bool justClicked,
    float deltaTime,
    float startX,
    float startY,
    int& traderPage,
    int& breweryPage,
    std::vector<traderItemsList>& traderItems,
    std::vector<potionsList>& potions,
    long double& bubbles,
    long double& realBubblesPerSecond,
    float traderRestockInterval,
    sf::Clock& traderRestockClock,
    std::unordered_map<std::string, float>& hoverScales
)
{
    std::vector<std::pair<std::string, MarketplaceTab>> subTabs = {
        { "Trader",  MarketplaceTab::Trader },
        { "Brewery", MarketplaceTab::Brewery }
        // Add more here later
    };

    sf::Vector2f tabSize(120.f, 40.f);
    float tabStartX = 10.f;
    float tabStartY = startY - 60.f;
    float tabSpacing = 10.f;

    for (size_t i = 0; i < subTabs.size(); i++) {
        auto [label, tab] = subTabs[i];
        sf::Vector2f pos(tabStartX, tabStartY + i * (tabSize.y + tabSpacing));

        bool hovered = sf::FloatRect(pos, tabSize).contains(mousePositionF);
        bool active = (currentMarketplaceTab == tab);

        sf::RectangleShape tabBox(tabSize);
        tabBox.setPosition(pos);
        tabBox.setFillColor(active ? sf::Color(200, 220, 255) :
            hovered ? sf::Color(230, 230, 230) : sf::Color(180, 180, 180));
        tabBox.setOutlineThickness(2.f);
        tabBox.setOutlineColor(sf::Color::Black);
        window.draw(tabBox);

        sf::Text tabText(font, label, 16);
        tabText.setFillColor(sf::Color::Black);

        // Center text inside the tab box
        sf::FloatRect textBounds = tabText.getLocalBounds();
        tabText.setPosition(
            { pos.x + (tabSize.x - textBounds.size.x) / 2.f - textBounds.position.x,
            pos.y + (tabSize.y - textBounds.size.y) / 2.f - textBounds.position.y }
        );
        window.draw(tabText);

        if (hovered && justClicked) {
            currentMarketplaceTab = tab;
        }
    }

    switch (currentMarketplaceTab) {
        case MarketplaceTab::Trader:
            renderTraderShop(window, font, mousePositionF, justClicked, deltaTime,
                             startX, startY, traderPage, traderItems,
                             bubbles, realBubblesPerSecond,
                             traderRestockInterval, traderRestockClock, hoverScales);
            break;

        case MarketplaceTab::Brewery:
            renderPotionBrewery(window, font, mousePositionF, justClicked, deltaTime,
                                startX, startY, breweryPage, potions,
                                traderItems, bubbles, hoverScales);
            break;

        // Future subtabs can be here
    }
}
