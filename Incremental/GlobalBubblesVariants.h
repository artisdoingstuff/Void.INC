#pragma once

#include "Includes.h"
#include "BuffTypes.h"

enum class globalBubbleVariantType
{
    Normal, Multiplicative, Additive,
    BubbleChaos, BubbleFrenzy, BubbleMayhem
};

struct globalBubbleBuffVariant
{
    float globalBubbleMultiplier;
    float globalBubbleDuration;
    sf::Color color;
    float globalBubbleRarity;
    buffVariantType buffType = buffVariantType::globalBubbleBuff;
    globalBubbleVariantType globalBubbleType = globalBubbleVariantType::Multiplicative;
    sf::Texture* texturePtr = nullptr;
};

// Texture references
extern sf::Texture bubbleTexture;
extern sf::Texture globalBubbleTexture;

inline vector<globalBubbleBuffVariant> globalBubbleVariants = {
    {
        2.0f, 30.0f,
        sf::Color::Blue,
        60.0f,
        buffVariantType::globalBubbleBuff,
        globalBubbleVariantType::Normal,
        &bubbleTexture
    },
    {
        3.0f, 20.0f,
        sf::Color::Blue,
        22.0f,
        buffVariantType::globalBubbleBuff,
        globalBubbleVariantType::Multiplicative,
        &globalBubbleTexture
    },
    {
        1.0f, 0.0f, 
        sf::Color::Blue,
        8.0f,
        buffVariantType::globalBubbleBuff,
        globalBubbleVariantType::Additive,
        & globalBubbleTexture
    },
    {
        0.6f, 14.0f,
        sf::Color::Blue,
        1.0f,
        buffVariantType::globalBubbleBuff,
        globalBubbleVariantType::BubbleChaos,
        & globalBubbleTexture
    },
    {
        1.5f, 20.0f,
        sf::Color::Blue,
        6.0f,
        buffVariantType::globalBubbleBuff,
        globalBubbleVariantType::BubbleFrenzy,
        & globalBubbleTexture
    },
    {
        0.9f, 20.0f,
        sf::Color::Blue,
        3.0f,
        buffVariantType::globalBubbleBuff,
        globalBubbleVariantType::BubbleMayhem,
        & globalBubbleTexture
    }
};

inline void selectGlobalBubbleVariant(
    globalBubbleBuffVariant& outVariant,
    sf::RectangleShape& hitbox,
    float& globalBubbleMultiplier,
    float& globalBubbleDuration,
    sf::Sprite& outSprite
)
{
    float totalRarity = 0.0f;
    for (const auto& variant : globalBubbleVariants)
        totalRarity += variant.globalBubbleRarity;

    float roll = static_cast<float>(rand()) / RAND_MAX * totalRarity;
    float cumulative = 0.0f;

    for (const auto& variant : globalBubbleVariants)
    {
        cumulative += variant.globalBubbleRarity;
        if (roll <= cumulative)
        {
            outVariant = variant;
            globalBubbleMultiplier = variant.globalBubbleMultiplier;
            globalBubbleDuration = variant.globalBubbleDuration;

            hitbox.setFillColor(variant.color);

            outSprite.setTexture(*variant.texturePtr, true);
            outSprite.setPosition(hitbox.getPosition());

            sf::Vector2f hitboxSize = hitbox.getSize();
            sf::Vector2u textureSize = variant.texturePtr->getSize();
            if (textureSize.x > 0 && textureSize.y > 0)
            {
                outSprite.setScale({
                    hitboxSize.x / static_cast<float>(textureSize.x),
                    hitboxSize.y / static_cast<float>(textureSize.y)
                    });
            }

            return;
        }
    }

    // Fallback
    outVariant = globalBubbleVariants.front();
    outSprite.setTexture(*outVariant.texturePtr, true);
    outSprite.setPosition(hitbox.getPosition());

    sf::Vector2f hitboxSize = hitbox.getSize();
    sf::Vector2u textureSize = outVariant.texturePtr->getSize();
    if (textureSize.x > 0 && textureSize.y > 0)
    {
        outSprite.setScale({
            hitboxSize.x / static_cast<float>(textureSize.x),
            hitboxSize.y / static_cast<float>(textureSize.y)
            });
    }
}