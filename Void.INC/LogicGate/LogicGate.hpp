#pragma once

#include "../Misc/GIncludes.hpp"

struct LogicGate {
    std::string name;
    std::string desc;
    long double baseBits;
    long double currentBits;
    long double bps;
    int ver = 0;
    sf::RectangleShape rect;

    LogicGate() : baseBits(0), currentBits(0), bps(0) {}

    LogicGate(std::string n, std::string d, long double bc, long double cc, long double bps)
        : name(n), desc(d), baseBits(bc), currentBits(cc), bps(bps) {
        rect.setSize(sf::Vector2f(350.f, 70.f));
        rect.setOutlineColor(sf::Color(243, 238, 225));
        rect.setOutlineThickness(2.f);
    }
};

inline std::vector<LogicGate> logicGateList;
inline float scrollOffset = 0.f;
inline float logicGateHeight = 70.f;
inline float logicGateSpacing = 10.f;
inline float uiWidth = 350.f;

inline void initLogicGates() {
    logicGateList.emplace_back("not_gate", "Inverts existence into non-existence.", 10.L, 10.L, 1.L);
    logicGateList.emplace_back("bit_flipper", "Forces ones into zeros.", 50.L, 50.L, 5.L);
    logicGateList.emplace_back("shift_register", "Pushes data into the abyss.", 100.L, 100.L, 10.L);
    logicGateList.emplace_back("null_comparator", "Validates the lack of data.", 250.L, 250.L, 25.L);
    logicGateList.emplace_back("bus_sniffer", "Captures and deletes stray signals.", 500.L, 500.L, 50.L);
    logicGateList.emplace_back("stack_siphon", "Drains the system’s memory reserves.", 1000.L, 1000.L, 100.L);
    logicGateList.emplace_back("parity_checker", "Corrects reality until it's empty.", 5000.L, 5000.L, 250.L);
}

inline void updateLogicGateUI(sf::RenderWindow& gameWindow, long double allBits) {
    const float margin = 40.f;
    float startX = gameWindow.getSize().x - uiWidth - margin;
    float startY = 105.f;

    int visibleCount = 0;
    for (size_t i = 0; i < logicGateList.size(); ++i) {
        if (logicGateList[i].ver > 0 || allBits >= logicGateList[i].baseBits) {
            float yPos = startY + (static_cast<float>(visibleCount) * (logicGateHeight + logicGateSpacing)) - scrollOffset;
            logicGateList[i].rect.setPosition({ startX, yPos });
            visibleCount++;
        }
    }
}

inline void to_json(json& j, const LogicGate& lg) {
    j = json{ {"n", lg.name}, {"v", lg.ver} };
}

inline void from_json(const json& j, LogicGate& lg) {
    j.at("n").get_to(lg.name);
    j.at("v").get_to(lg.ver);
}