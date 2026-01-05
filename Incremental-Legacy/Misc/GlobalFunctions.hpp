#pragma once

#include "Includes.hpp"
#include "GlobalVariables.hpp"

struct FloatingText
{
    sf::Text drawable;
    sf::Color baseColor;
    sf::Vector2f velocity;
    float lifetime;

    FloatingText(const string& text, sf::Font& font, sf::Vector2f pos)
        : drawable(font), velocity(0.0f, -30.0f), lifetime(1.5f)
    {
        drawable.setCharacterSize(20);
        drawable.setString(text);
        baseColor = sf::Color::Yellow;
        drawable.setFillColor(baseColor);
        drawable.setOutlineColor(sf::Color::Black);
        drawable.setOutlineThickness(2.0f);
        drawable.setPosition(pos);
    }

    void update(float dt)
    {
        drawable.move(velocity * dt);
        lifetime -= dt;

        float alpha = max(0.0f, lifetime / 1.5f);
        sf::Color fade = baseColor;
        fade.a = static_cast<int>(alpha * 255);
        drawable.setFillColor(fade);
    }

    bool isExpired() const { return lifetime <= 0.0f; }
};

inline float randomFloat(float min, float max)
{
    return min + static_cast<float>(rand()) / RAND_MAX * (max - min);
}