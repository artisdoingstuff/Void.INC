#pragma once
#include "GIncludes.hpp"

inline void hideConsole() {
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

inline sf::Clock cursorClock;
inline std::string getCursor() {
    return (cursorClock.getElapsedTime().asMilliseconds() % 1000 < 500) ? "_" : " ";
}

inline void centerText(sf::Text& text, sf::Vector2f targetPos) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin({ textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f });
    text.setPosition(targetPos);
}