#pragma once

#include "../Misc/GIncludes.hpp"

struct RootPatch {
    std::string name;
    std::string desc;
    long double bytes;
    int patched = 0;
    sf::RectangleShape rect;

    RootPatch(std::string n, std::string t, long double c) : name(n), desc(t), bytes(c) {
        rect.setSize({ 300.f, 60.f });
        rect.setFillColor(sf::Color(10, 20, 10, 200));
        rect.setOutlineColor(sf::Color(0, 255, 150));
        rect.setOutlineThickness(2.f);
    }
};

inline std::vector<RootPatch> rootTree;

inline void initRootTree() {
    if (!rootTree.empty()) return;
    rootTree.emplace_back("Patch_0", "Upgrade bit-to-byte protocols (1e-6).", 0.05L);
    rootTree.emplace_back("Patch_1", "Inject minute-based variance (x1.5 - x1.7).", 10.0L);
    rootTree.emplace_back("Patch_2", "Triples bits and doubles clicks.", 50.0L);
    rootTree.emplace_back("Patch_3_1", "x5.5 BPS but lowers byte rate to 3e-7.", 500.0L);
    rootTree.emplace_back("Patch_3_2", "Byte-scaled feedback loop (0.2% per Byte).", 1000.0L);
}

inline void drawRootDirectory(sf::RenderWindow& window, long double currentBytes) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    float startX = 100.f;
    float startY = 150.f;

    for (size_t i = 0; i < rootTree.size(); ++i) {
        bool isUnlocked = (i == 0) || (i < 3 && rootTree[i - 1].patched) || (i >= 3 && rootTree[2].patched);

        if (isUnlocked) {
            float xOffset = (i >= 3) ? (i - 3) * 320.f : 0;
            float yPos = startY + (std::min((int)i, 3) * 80.f);
            rootTree[i].rect.setPosition({ startX + xOffset, yPos });

            std::string statusText;
            if (rootTree[i].patched) {
                statusText = rootTree[i].name + ".sys\n[ PATCHED ]";
                rootTree[i].rect.setOutlineColor(sf::Color(50, 100, 75));
                rootTree[i].rect.setFillColor(sf::Color(5, 10, 5, 150));
            }
            else {
                std::stringstream stream;
                stream << std::fixed << std::setprecision(2) << rootTree[i].bytes;
                statusText = rootTree[i].name + ".sys\nBytes: -" + stream.str() + " Bytes";
                rootTree[i].rect.setOutlineColor(sf::Color(0, 255, 150));

                if (rootTree[i].rect.getGlobalBounds().contains(mousePos)) {
                    rootTree[i].rect.setFillColor(sf::Color(20, 60, 40));

                    sf::Text tip(jetBrainsMono, rootTree[i].desc, 14);
                    tip.setPosition(mousePos + sf::Vector2f(15, 15));
                    window.draw(tip);
                }
                else {
                    rootTree[i].rect.setFillColor(sf::Color(0, 0, 0, 180));
                }
            }

            window.draw(rootTree[i].rect);
            sf::Text txt(jetBrainsMono, statusText, 16);
            txt.setPosition(rootTree[i].rect.getPosition() + sf::Vector2f(10, 5));
            window.draw(txt);
        }
    }

    sf::RectangleShape initBtn({ 140.f, 50.f });
    initBtn.setPosition(sf::Vector2f(window.getSize().x - 160.f, window.getSize().y - 70.f));
    initBtn.setFillColor(sf::Color(80, 0, 0));
    initBtn.setOutlineColor(sf::Color::Red);
    initBtn.setOutlineThickness(2.f);
    if (initBtn.getGlobalBounds().contains(mousePos)) {
        initBtn.setFillColor(sf::Color::Red);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && canClickInit) {
            currentInitStep = InitState::IDLE;
            initialization = false;
            canClickInit = false;
            canClick = true;
        }
    }
    window.draw(initBtn);

    sf::Text initTxt(jetBrainsMono, "INIT();", 22);
    initTxt.setPosition(initBtn.getPosition() + sf::Vector2f(30, 10));
    window.draw(initTxt);
}

inline void to_json(json& j, const RootPatch& p) {
    j = json{ {"n", p.name}, {"p", p.patched} };
}

inline void from_json(const json& j, RootPatch& p) {
    j.at("n").get_to(p.name);
    j.at("p").get_to(p.patched);
}