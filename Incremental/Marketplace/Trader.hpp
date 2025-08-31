#pragma once

#include "Misc/Includes.hpp"
#include "Buffs/GlobalBuffHandler.hpp"
#include "Misc/GlobalVariables.hpp"
#include "Misc/Popup.hpp"
#include "Buffs/Trader/TradersList.hpp"

inline long double getEffectiveItemCost(const traderItemsList& item, long double currentBps)
{
    if (item.dynamicPricing)
    {
        long double scaledBase = item.dynamicPriceMultiplier * currentBps;
        return round(scaledBase * pow(dynamicInflationRate, item.timesBought));
    }

    return item.cost;
}

inline void tryPurchaseItem(traderItemsList& item, long double& currentBubbles, long double currentBps)
{
    if (!item.unlocked || !item.visible || item.isActive || item.cooldownRemaining > 0.f)
        return;

    if (item.stockRemaining == 0)
        return;

    long double effectiveCost = getEffectiveItemCost(item, currentBps);

    if (currentBubbles < effectiveCost)
        return;

    currentBubbles -= effectiveCost;

    item.timesBought++;

    if (!item.dynamicPricing)
        item.cost = round(item.baseCost * pow(itemInflationRate, item.timesBought));

    item.isActive = true;
    item.remaining = item.duration;
    item.cooldownRemaining = item.cooldownDuration;

    if (item.maxStock != -1)
        item.stockRemaining--;

    if (item.onActivate)
        item.onActivate();
}

inline bool tryManualRestock(traderItemsList& item, long double& currentBubbles)
{
    if (!item.canBeRestockedManually || item.stockRemaining == item.maxStock)
        return false;

    if (currentBubbles < item.restockCost)
        return false;

    currentBubbles -= item.restockCost;
    item.stockRemaining = item.maxStock;
    return true;
}

inline void upgradeTrader(float dt)
{
    for (auto& item : traderItems)
    {
        if (item.isActive)
        {
            item.remaining -= dt;
            if (item.remaining <= 0.0f)
            {
                item.isActive = false;
                item.remaining = 0.0f;

                if (item.onExpire) item.onExpire();
            }
        }

        if (item.cooldownRemaining > 0.0f)
        {
            item.cooldownRemaining -= dt;
            if (item.cooldownRemaining < 0.0f)
                item.cooldownRemaining = 0.0f;
        }
    }
}

inline void randomlyRestockTraderItems()
{
    for (auto& item : traderItems)
    {
        if (item.maxStock > 0 && item.stockRemaining < item.maxStock && item.restockChance > 0.f)
        {
            float roll = static_cast<float>(rand()) / RAND_MAX;
            if (roll < item.restockChance)
            {
                item.stockRemaining++;
            }
        }
    }
}

inline void renderTraderShop(
    sf::RenderWindow& window,
    const sf::Font& font,
    const sf::Vector2f& mousePositionF,
    bool justClicked,
    float deltaTime,
    float startX,
    float startY,
    int& traderPage,
    vector<traderItemsList>& traderItems,
    long double bubbles,
    long double realBubblesPerSecond,
    float traderRestockInterval,
    sf::Clock& traderRestockClock,
    unordered_map<string, float>& hoverScales
) {
    constexpr int itemsPerPage = 5;
    int totalPages = (traderItems.size() + itemsPerPage - 1) / itemsPerPage;
    traderPage = std::clamp(traderPage, 0, std::max(0, totalPages - 1));

    int startIdx = traderPage * itemsPerPage;
    int endIdx = std::min<int>(startIdx + itemsPerPage, traderItems.size());

    constexpr float buffBoxWidth = 240.f;
    constexpr float buffBoxHeight = 80.f;
    constexpr float buffSpacingY = 20.f;

    float buffStartY = startY;
    float buffStartX = startX - buffBoxWidth + 150.f;

    for (int i = startIdx; i < endIdx; i++)
    {
        auto& item = traderItems[i];
        sf::Vector2f pos(buffStartX, buffStartY);
        sf::RectangleShape box({ buffBoxWidth, buffBoxHeight });
        box.setOrigin({ buffBoxWidth / 2.f, buffBoxHeight / 2.f });
        box.setPosition(pos + sf::Vector2f(buffBoxWidth / 2.f, buffBoxHeight / 2.f));

        bool isHovered = sf::FloatRect(pos, { buffBoxWidth, buffBoxHeight }).contains(mousePositionF);
        float& currentScale = hoverScales[item.name];
        float targetScale = isHovered ? 1.05f : 1.0f;
        float lerpSpeed = 8.0f * deltaTime;
        currentScale += (targetScale - currentScale) * lerpSpeed;
        currentScale = clamp(currentScale, 1.0f, 1.1f);
        box.setScale({ currentScale, currentScale });

        bool canAfford = bubbles >= getEffectiveItemCost(item, realBubblesPerSecond);
        bool isOnCooldown = item.cooldownRemaining > 0.f;
        bool isSoldOut = (item.maxStock != -1 && item.stockRemaining <= 0);

        // Fill color logic
        if (isSoldOut)
            box.setFillColor(sf::Color(80, 80, 80));
        else if (isOnCooldown)
            box.setFillColor(sf::Color(100, 100, 100));
        else if (isHovered)
            box.setFillColor(sf::Color(255, 255, 200));
        else if (canAfford)
            box.setFillColor(sf::Color(210, 250, 210));
        else
            box.setFillColor(sf::Color(120, 120, 120));

        box.setOutlineThickness(2.f);
        box.setOutlineColor(sf::Color(180, 180, 180));
        window.draw(box);

        // Name
        sf::Text nameText(font);
        nameText.setCharacterSize(16);
        nameText.setString(item.name);
        nameText.setFillColor(sf::Color::Black);
        nameText.setPosition(pos + sf::Vector2f(12.f, 6.f));
        window.draw(nameText);

        // Description
        sf::Text descText(font);
        descText.setCharacterSize(12);
        descText.setString(item.description);
        descText.setFillColor(sf::Color::Black);
        descText.setPosition(pos + sf::Vector2f(12.f, 28.f));
        window.draw(descText);

        // Cost or cooldown or sold out
        sf::Text costOrTimerText(font);
        costOrTimerText.setCharacterSize(14);
        costOrTimerText.setFillColor(sf::Color::Black);

        if (isSoldOut)
        {
            costOrTimerText.setString("Sold Out");
        }
        else if (isOnCooldown)
        {
            int seconds = static_cast<int>(ceilf(item.cooldownRemaining));
            costOrTimerText.setString("Cooldown: " + to_string(seconds) + "s");
        }
        else
        {
            costOrTimerText.setString("Cost: " + formatDisplayBubbles(getEffectiveItemCost(item, realBubblesPerSecond)) + " Bubbles");
        }

        costOrTimerText.setPosition(pos + sf::Vector2f(12.f, 55.f));
        window.draw(costOrTimerText);

        // Stock display
        if (item.maxStock != -1)
        {
            sf::Text stockText(font);
            stockText.setCharacterSize(12);
            stockText.setFillColor(sf::Color::Black);
            stockText.setString("Stock: " + to_string(item.stockRemaining) + "/" + to_string(item.maxStock));
            stockText.setPosition(pos + sf::Vector2f(buffBoxWidth - 92.f, 6.f));
            window.draw(stockText);
        }

        // Manual Restock Button
        if (isSoldOut && item.canBeRestockedManually)
        {
            sf::Vector2f buttonSize = { 65.f, 28.f };
            sf::Vector2f buttonPos = pos + sf::Vector2f(buffBoxWidth - buttonSize.x - 12.f, buffBoxHeight - buttonSize.y - 8.f);

            bool buttonHovered = sf::FloatRect(buttonPos, buttonSize).contains(mousePositionF);

            sf::Color buttonFill = buttonHovered ? sf::Color::Black : sf::Color::White;
            sf::Color textColor = buttonHovered ? sf::Color::White : sf::Color::Black;

            sf::RectangleShape restockButton(buttonSize);
            restockButton.setPosition(buttonPos);
            restockButton.setFillColor(buttonFill);
            restockButton.setOutlineThickness(1.f);
            restockButton.setOutlineColor(sf::Color(60, 60, 60));
            window.draw(restockButton);

            sf::Text buttonText(font);
            buttonText.setCharacterSize(12);
            buttonText.setFillColor(textColor);
            buttonText.setString("Restock");
            buttonText.setPosition(buttonPos + sf::Vector2f(10.f, 5.f));
            window.draw(buttonText);

            if (justClicked && buttonHovered)
            {
                item.stockRemaining = item.maxStock;
            }
        }

        // Purchase
        if (isHovered && justClicked && canAfford && !isOnCooldown && !isSoldOut)
        {
            tryPurchaseItem(item, bubbles, realBubblesPerSecond);
        }

        buffStartY += buffBoxHeight + buffSpacingY;
    }

    float navY = buffStartY + 20.f;
    sf::Vector2f navSize = { 80.f, 30.f };
    sf::Vector2f prevPos = { buffStartX, navY };
    sf::Vector2f nextPos = { buffStartX + buffBoxWidth - navSize.x, navY };

    // Prev button
    sf::RectangleShape prevButton(navSize);
    prevButton.setPosition(prevPos);
    prevButton.setFillColor(traderPage > 0 ? sf::Color(180, 180, 180) : sf::Color(100, 100, 100));
    window.draw(prevButton);

    sf::Text prevText(font, "Prev", 14);
    prevText.setFillColor(sf::Color::Black);
    prevText.setPosition(prevPos + sf::Vector2f(10.f, 5.f));
    window.draw(prevText);

    // Next button
    sf::RectangleShape nextButton(navSize);
    nextButton.setPosition(nextPos);
    nextButton.setFillColor((traderPage + 1) < totalPages ? sf::Color(180, 180, 180) : sf::Color(100, 100, 100));
    window.draw(nextButton);

    sf::Text nextText(font, "Next", 14);
    nextText.setFillColor(sf::Color::Black);
    nextText.setPosition(nextPos + sf::Vector2f(10.f, 5.f));
    window.draw(nextText);

    // Handle clicks
    if (justClicked)
    {
        if (sf::FloatRect(prevPos, navSize).contains(mousePositionF) && traderPage > 0)
            traderPage--;
        if (sf::FloatRect(nextPos, navSize).contains(mousePositionF) && traderPage + 1 < totalPages)
            traderPage++;
    }

    // Restock timer under buttons
    float restockTimeLeft = std::max(0.f, traderRestockInterval - traderRestockClock.getElapsedTime().asSeconds());
    sf::Text restockTimerText(font);
    restockTimerText.setCharacterSize(14);
    restockTimerText.setFillColor(sf::Color::Black);
    restockTimerText.setString("Next Restock: " + std::to_string(static_cast<int>(ceilf(restockTimeLeft))) + "s");
    restockTimerText.setPosition({ buffStartX, navY + 50.f });
    window.draw(restockTimerText);
}

inline void to_json(json& j, const traderItemsList& item)
{
    j = json{
        { "name", item.name },
        { "cost", item.cost },
        { "timesBought", item.timesBought },
        { "unlocked", item.unlocked },
        { "visible", item.visible },
        { "stockRemaining", item.stockRemaining },
        { "canBeRestockedManually", item.canBeRestockedManually },
        { "restockCost", item.restockCost },
        { "restockChance", item.restockChance },
        { "dynamicPricing", item.dynamicPricing },
        { "dynamicPriceMultiplier", item.dynamicPriceMultiplier },
        { "cooldownRemaining", item.cooldownRemaining },
        { "remaining", item.remaining }
    };
}

inline void from_json(const json& j, traderItemsList& item)
{
    j.at("cost").get_to(item.cost);
    j.at("timesBought").get_to(item.timesBought);
    if (j.contains("unlocked")) j.at("unlocked").get_to(item.unlocked);
    if (j.contains("visible"))  j.at("visible").get_to(item.visible);
    if (j.contains("stockRemaining")) j.at("stockRemaining").get_to(item.stockRemaining);
    if (j.contains("canBeRestockedManually")) j.at("canBeRestockedManually").get_to(item.canBeRestockedManually);
    if (j.contains("restockCost")) j.at("restockCost").get_to(item.restockCost);
    if (j.contains("restockChance")) j.at("restockChance").get_to(item.restockChance);
    if (j.contains("dynamicPricing")) j.at("dynamicPricing").get_to(item.dynamicPricing);
    if (j.contains("dynamicPriceMultiplier")) j.at("dynamicPriceMultiplier").get_to(item.dynamicPriceMultiplier);
    if (j.contains("cooldownRemaining")) j.at("cooldownRemaining").get_to(item.cooldownRemaining);
    if (j.contains("remaining")) j.at("remaining").get_to(item.remaining);

    auto it = restockOverrideMap.find(item.name);
    if (it != restockOverrideMap.end()) {
        item.restockChance = it->second;
    }

    auto dpOverride = dynamicPricingOverrideMap.find(item.name);
    if (dpOverride != dynamicPricingOverrideMap.end()) {
        item.dynamicPriceMultiplier = dpOverride->second;
    }
}