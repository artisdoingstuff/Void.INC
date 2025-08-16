#pragma once

#include "Misc/Includes.hpp"

struct ShootingStar {
    optional<sf::Sprite> sprite;
    sf::Vector2f start;
    sf::Vector2f end;
    float height;
    float duration;
    sf::Clock clock;
    bool collected = false;

    ShootingStar(sf::Texture& tex, sf::Vector2f start, sf::Vector2f end, float height, float duration)
        : start(start), end(end), height(height), duration(duration)
    {
        sf::Sprite temp(tex);
        temp.setOrigin({ tex.getSize().x / 2.0f, tex.getSize().y / 2.0f });
        temp.setPosition(start);
        temp.setScale({ 0.5f, 0.5f });

        sprite = temp;
    }

    void update()
    {
        float t = clock.getElapsedTime().asSeconds() / duration;
        if (t > 1.0f) t = 1.0f;

        float x = start.x + (end.x - start.x) * t;
        float y = start.y + (end.y - start.y) * t - height * 4 * t * (1 - t);

        if (sprite.has_value())
            sprite->setPosition({ x, y });
    }

    bool isExpired() const {
        return collected || clock.getElapsedTime().asSeconds() >= duration;
    }

    bool isClicked(const sf::Vector2f& mousePos) const {
        if (!sprite) return false;

        const sf::FloatRect bounds = sprite->getTransform().transformRect(sprite->getLocalBounds());
        return bounds.contains(mousePos);
    }
};