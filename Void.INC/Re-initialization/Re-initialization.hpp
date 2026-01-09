#pragma once

#include "Misc/GIncludes.hpp"

inline void resetProgress() {
    for (auto& lg : logicGateList) {
        lg.ver = 0;
        lg.currentBits = lg.baseBits;
    }

    for (auto& hf : hotfixList) {
        hf.written = 0;
    }

    bits = 0.0L;
    bitsPerSecond = 0.0L;
    hotfixMult = 1.0L;
    scrollOffset = 0.0f;
}

inline void drawLoadingUI(sf::RenderWindow& window, float actualProgress) {
    float width = 600.f;
    float height = 5.f;
    sf::Vector2f center = { window.getSize().x / 2.f, window.getSize().y / 2.f };

    float time = std::clock() / static_cast<float>(CLOCKS_PER_SEC);
    float jitter = std::sin(time * 2.f) * 0.05f + std::sin(time * 5.f) * 0.02f;

    float visualProgress = actualProgress + jitter;
    if (visualProgress > actualProgress) visualProgress = actualProgress;
    if (visualProgress < 0.f) visualProgress = 0.f;

    if ((actualProgress > 0.35f && actualProgress < 0.45f) || (actualProgress > 0.85f && actualProgress < 0.98f)) {
        visualProgress -= 0.05f;
    }

    static std::string currentStatus = "Initializing...";
    static float lastUpdate = 0.f;

    std::vector<std::string> messages = {
        "Purging Bits...", "Realigning Vortex...", "Cleaning Cache...",
        "Defragmenting Void...", "Recalculating Reality...", "Patching Void.exe...",
        "Formatting Logic...", "Flushing Memory...", "Bypassing Hotfixes..."
    };

    if (actualProgress < 0.85f) {
        if (time - lastUpdate > 2.f) {
            currentStatus = messages[std::rand() % messages.size()];
            lastUpdate = time;
        }
    }
    else {
        currentStatus = "Opening Root.dir...";
    }

    sf::RectangleShape bg({ width, height });
    bg.setOrigin({ width / 2.f, height / 2.f });
    bg.setPosition(center);
    bg.setFillColor(sf::Color(30, 30, 30));
    window.draw(bg);

    sf::RectangleShape fill({ width * std::clamp(visualProgress, 0.f, 1.f), height });
    fill.setOrigin({ 0.f, height / 2.f });
    fill.setPosition(sf::Vector2f(center.x - (width / 2.f), center.y));
    fill.setFillColor(sf::Color(243, 238, 225));
    window.draw(fill);

    sf::Text pct(jetBrainsMono);
    pct.setString(std::to_string(static_cast<int>(std::clamp(visualProgress, 0.f, 1.f) * 100)) + "%");
    pct.setCharacterSize(16);
    pct.setFillColor(sf::Color(243, 238, 225));
    pct.setPosition(sf::Vector2f(center.x - 15.f, center.y + 20.f));
    window.draw(pct);

    sf::Text status(jetBrainsMono);
    status.setString(currentStatus);
    status.setCharacterSize(14);
    status.setFillColor(sf::Color(150, 150, 150));
    status.setPosition(sf::Vector2f(center.x - (status.getGlobalBounds().size.x / 2.f), center.y - 30.f));
    window.draw(status);
}

inline long double getPendingBytes(long double bits) {
    auto round = [](long double v) -> long double {return std::round(v * 100.0) / 100.0; };
    if (bits < 5000000.0L) return 0;
    return round(bits * bitsToBytesRate);
}

inline void drawConfirmPopup(sf::RenderWindow& window, bool& startInit) {
    sf::RectangleShape overlay({ (float)window.getSize().x, (float)window.getSize().y });
    overlay.setFillColor(sf::Color(20, 20, 20, 200));

    sf::RectangleShape box({ 400.f, 200.f });
    box.setOrigin({ 200.f, 100.f });
    box.setPosition({ window.getSize().x / 2.f, window.getSize().y / 2.f });
    box.setFillColor(sf::Color(20, 20, 20));
    box.setOutlineColor(sf::Color::Red);
    box.setOutlineThickness(2);

    sf::Text warnText(jetBrainsMono, "REINITIALIZING WILL WIPE\nALL CURRENT DATA.\n\nPROCEED?", 18);
    warnText.setOrigin({ warnText.getGlobalBounds().size.x / 2.f, 0.f });
    warnText.setPosition(sf::Vector2f(box.getPosition().x, box.getPosition().y - 70.f));

    sf::Text yesText(jetBrainsMono, "[ YES ]", 18);
    yesText.setPosition(sf::Vector2f(box.getPosition().x - 100.f, box.getPosition().y + 40.f));
    yesText.setFillColor(sf::Color::Green);

    sf::Text noText(jetBrainsMono, "[ NO ]", 18);
    noText.setPosition(sf::Vector2f(box.getPosition().x + 20.f, box.getPosition().y + 40.f));
    noText.setFillColor(sf::Color::Red);

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    window.draw(overlay);
    window.draw(box);
    window.draw(warnText);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        if (yesText.getGlobalBounds().contains(mousePos)) {
            startInit = true;
            bytes += getPendingBytes(bits);
            resetProgress();
            showConfirmPopup = false;
        }
        if (noText.getGlobalBounds().contains(mousePos)) {
            showConfirmPopup = false;
        }
    }

    window.draw(yesText);
    window.draw(noText);
}