#pragma once

#include "Includes.h"

enum class buffVariantType {
    globalBubbleBuff,
    rubberDuckBuff,
    mutationBuff
};

enum class GlobalBuffType {
    Normal,
    Multiplicative,
    Additive,
    Chaos,
    Frenzy,
    Mayhem,
    Mutated,
    ShootingStar
};

struct GlobalBuffVariant {
    GlobalBuffType effectType;
    float multiplier;
    float duration;
    float rarity;
    optional<sf::Texture*> textureOpt;
    buffVariantType variantType = buffVariantType::globalBubbleBuff;
};

extern sf::Texture bubbleTexture;
extern sf::Texture goldenBubbleTexture;

inline vector<GlobalBuffVariant> globalBuffVariants = {
    { GlobalBuffType::Normal,         2.0f, 30.0f, 60.0f, &bubbleTexture,       buffVariantType::globalBubbleBuff },
    { GlobalBuffType::Multiplicative, 3.0f, 20.0f, 22.0f, &goldenBubbleTexture, buffVariantType::globalBubbleBuff },
    { GlobalBuffType::Additive,       1.0f,  0.0f,  8.0f, &goldenBubbleTexture, buffVariantType::globalBubbleBuff },
    { GlobalBuffType::Chaos,          0.6f, 14.0f,  1.0f, &goldenBubbleTexture, buffVariantType::globalBubbleBuff },
    { GlobalBuffType::Frenzy,         1.5f, 20.0f,  5.0f, &goldenBubbleTexture, buffVariantType::globalBubbleBuff },
    { GlobalBuffType::Mayhem,         0.9f, 20.0f,  2.0f, &goldenBubbleTexture, buffVariantType::globalBubbleBuff },
    { GlobalBuffType::Mutated,        1.0f, 25.0f,  0.0f, &goldenBubbleTexture, buffVariantType::mutationBuff },
    { GlobalBuffType::ShootingStar,   5.0f, 10.0f,  2.0f, &goldenBubbleTexture, buffVariantType::globalBubbleBuff },
    
};

struct ActiveGlobalBuff {
    sf::Sprite sprite;
    GlobalBuffType effectType;
    buffVariantType originType;
    float remaining;
    int variantIndex;
    bool active = true;
    sf::Clock pulseClock;

    ActiveGlobalBuff(const sf::Sprite& s, GlobalBuffType eff, buffVariantType origin, float dur, int index)
        : sprite(s), effectType(eff), originType(origin), remaining(dur), variantIndex(index) {
    }
};

inline vector<ActiveGlobalBuff> activeGlobalBuffs;

struct QueuedGlobalBuff {
    float delayRemaining = 0.0f;
};

inline deque<QueuedGlobalBuff> globalBuffQueue;
inline sf::Clock globalBuffClock;
inline const float minBuffCooldown = 200.0f;
inline const float maxBuffCooldown = 300.0f;

inline void queueGlobalBuffs(int count)
{
    float spawnRateMult = clamp(perkManager.buffSpawnRateMultiplier, 0.1f, 3.0f);
    float baseDelay = (static_cast<float>(rand()) / RAND_MAX * (maxBuffCooldown - minBuffCooldown) + minBuffCooldown);
    baseDelay /= spawnRateMult;

    for (int i = 0; i < count; ++i)
        globalBuffQueue.push_back({ baseDelay * (i + 1) });
}

inline bool shouldSpawnGlobalBuff()
{
    if (globalBuffQueue.empty()) return false;

    float elapsed = globalBuffClock.getElapsedTime().asSeconds();
    if (elapsed >= globalBuffQueue.front().delayRemaining)
    {
        globalBuffQueue.pop_front();
        globalBuffClock.restart();
        return true;
    }
    return false;
}

inline optional<pair<GlobalBuffVariant, sf::Sprite>> selectGlobalBuffVariant(sf::Vector2f pos, sf::Vector2f size)
{
    float totalRarity = 0.0f;
    for (auto& v : globalBuffVariants) totalRarity += v.rarity;
    float roll = static_cast<float>(rand()) / RAND_MAX * totalRarity;

    float cumulative = 0.0f;
    for (size_t i = 0; i < globalBuffVariants.size(); ++i)
    {
        cumulative += globalBuffVariants[i].rarity;
        if (roll <= cumulative)
        {
            GlobalBuffVariant outVariant = globalBuffVariants[i];
            if (!outVariant.textureOpt.has_value()) return nullopt;

            sf::Texture* tex = outVariant.textureOpt.value();
            sf::Sprite outSprite(*tex);
            outSprite.setPosition(pos);

            sf::Vector2u texSize = tex->getSize();
            if (texSize.x > 0 && texSize.y > 0)
            {
                outSprite.setScale({ size.x / texSize.x, size.y / texSize.y });
            }

            return make_pair(outVariant, outSprite);
        }
    }

    return nullopt;
}

inline void spawnGlobalBuff(const sf::Sprite& sprite, int variantIndex)
{
    const auto& variant = globalBuffVariants[variantIndex];
    activeGlobalBuffs.emplace_back(sprite, variant.effectType, variant.variantType, variant.duration, variantIndex);
}

inline void updateGlobalBuffs(float dt)
{
    for (auto& buff : activeGlobalBuffs)
    {
        if (!buff.active) continue;

        buff.remaining -= dt;
        if (buff.remaining <= 0.0f)
        {
            buff.active = false;
            continue;
        }

        float alpha = clamp(buff.remaining / 1.5f, 0.f, 1.f);
        sf::Color c = buff.sprite.getColor();
        c.a = static_cast<int>(alpha * 255);
        buff.sprite.setColor(c);

        float t = buff.pulseClock.getElapsedTime().asMilliseconds();
        float pulse = 1.0f + 0.02f * sinf(t * 0.003f);
        buff.sprite.setScale({ pulse * 0.8f, pulse * 0.8f });

		sf::Vector2u textureSize = buff.sprite.getTexture().getSize();
        buff.sprite.setOrigin({ textureSize.x / 2.0f, textureSize.y / 2.0f });
    }

    activeGlobalBuffs.erase(remove_if(activeGlobalBuffs.begin(), activeGlobalBuffs.end(),
        [](const ActiveGlobalBuff& b) { return !b.active; }),
        activeGlobalBuffs.end());
}

inline void handleGlobalBuffClicks(sf::Vector2f mousePos,
    const function<void(const GlobalBuffVariant&)>& onClick)
{
    for (auto& buff : activeGlobalBuffs)
    {
        if (!buff.active) continue;

        if (buff.sprite.getGlobalBounds().contains(mousePos))
        {
            onClick(globalBuffVariants[buff.variantIndex]);
            buff.active = false;
        }
    }
}

inline void drawGlobalBuffs(sf::RenderWindow& window)
{
    for (const auto& buff : activeGlobalBuffs)
    {
        if (buff.active)
            window.draw(buff.sprite);
    }
}