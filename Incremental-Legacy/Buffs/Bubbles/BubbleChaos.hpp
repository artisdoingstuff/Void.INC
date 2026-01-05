#pragma once

#include "Misc/Includes.hpp"

extern sf::Texture bubbleTexture;

struct BubbleChaos
{
    sf::RectangleShape hitbox;

    sf::Sprite bubbleSprite;

    sf::Vector2f position;
    sf::Clock bubbleMayhemLifetimeClock;
    float bubbleLifetime;

    sf::Vector2f originalBubbleScale;
    sf::Clock bubblePoppingClock;
    bool isBubblePopping;
    float bubblePoppingDuration;

    BubbleChaos(sf::Vector2f position)
        : position(position), bubbleLifetime(5.0f), bubbleSprite(bubbleTexture),
        isBubblePopping(false), bubblePoppingDuration(0.3f)
    {

        float randomBubbleSize = static_cast<float>((rand() % 41) + 50);
        sf::Vector2f size(randomBubbleSize, randomBubbleSize);

        bubbleSprite.setTexture(bubbleTexture);
        bubbleSprite.setPosition(position);
        bubbleSprite.setScale(sf::Vector2f(size.x / bubbleTexture.getSize().x, size.y / bubbleTexture.getSize().y));

        sf::Vector2f textureSize(bubbleTexture.getSize().x, bubbleTexture.getSize().y);
        bubbleSprite.setOrigin(sf::Vector2f(textureSize.x / 2.0f, textureSize.y / 2.0f));

        originalBubbleScale = bubbleSprite.getScale();

        hitbox.setSize(size);
        hitbox.setPosition(sf::Vector2f(position.x - size.x / 2.0f, position.y - size.y / 2.0f));
        hitbox.setFillColor(sf::Color::Transparent);
    }

    bool isTimeExpired() const
    {
        return bubbleMayhemLifetimeClock.getElapsedTime().asSeconds() >= bubbleLifetime;
    }

    void updateBubbleAlpha()
    {
        float elapsed = bubbleMayhemLifetimeClock.getElapsedTime().asSeconds();

        float fadeInTime = 0.3f;
        float fadeOutTime = 0.3f;

        float alpha = 255.0f;

        if (elapsed < fadeInTime)
        {
            alpha = (elapsed / fadeInTime) * 255.0f;
        }
        else if (elapsed > bubbleLifetime - fadeOutTime)
        {
            float remaining = bubbleLifetime - elapsed;
            alpha = (remaining / fadeOutTime) * 255.0f;
        }

        if (alpha < 0.0f) alpha = 0.0f;
        if (alpha > 255.0f) alpha = 255.0f;

        sf::Color color = bubbleSprite.getColor();
        color.a = static_cast<int>(alpha);
        bubbleSprite.setColor(color);
    }

    void startPoppingBubbles()
    {
        isBubblePopping = true;
        bubblePoppingClock.restart();
    }

    void updatePoppingBubblesAnimation()
    {
        if (!isBubblePopping) return;

        float elapsed = bubblePoppingClock.getElapsedTime().asSeconds();
        float progress = elapsed / bubblePoppingDuration;

        if (progress >= 1.0f)
        {
            return;
        }

        float scale = 1.0f - progress;

        bubbleSprite.setScale(sf::Vector2f(originalBubbleScale.x * scale, originalBubbleScale.y * scale));

        float alpha = 255.0f * scale;
        sf::Color color = bubbleSprite.getColor();
        color.a = static_cast<int>(alpha);
        bubbleSprite.setColor(color);
    }

    bool isPoppingBubblesComplete() const
    {
        return isBubblePopping && bubblePoppingClock.getElapsedTime().asSeconds() >= bubblePoppingDuration;
    }
};