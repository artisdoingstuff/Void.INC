#pragma once

#include "Misc/Includes.hpp"
#include "Misc/GlobalFunctions.hpp"

struct Ripple
{
    sf::CircleShape circle;
    float lifetime = 0.5f;
    float maxLifetime = 0.5f;
    float maxRadius = 150.0f;

    Ripple(sf::Vector2f pos)
    {
        circle.setRadius(0.0f);
        circle.setOrigin({ 0.0f, 0.0f });
        circle.setPosition(pos);
        circle.setFillColor(sf::Color(255, 255, 0, 200));
    }

    void update(float dt)
    {
        lifetime -= dt;
        float progress = 1.0f - (lifetime / maxLifetime);

        float radius = progress * maxRadius;
        circle.setRadius(radius);
        circle.setOrigin({ radius, radius });

        sf::Color c = circle.getFillColor();
        c.a = static_cast<int>(100 * (1.0f - progress));
        circle.setFillColor(c);
    }

    bool isExpired() const { return lifetime <= 0.0f; }
};

class ComboSystem
{
public:
    void initializeUI(const sf::Vector2u& windowSize)
    {
        comboBarBackground.setSize({ 200.0f, 20.0f });
        comboBarBackground.setPosition({ 20.0f, 20.0f });
        comboBarBackground.setFillColor(sf::Color(50, 50, 50));

        comboBarFill.setSize({ 0.0f, 20.0f });
        comboBarFill.setPosition(comboBarBackground.getPosition());
        comboBarFill.setFillColor(sf::Color(100, 200, 255));
    }

    void update(float deltaTime)
    {
        if (comboOnCooldown)
        {
            comboCooldownTimer -= deltaTime;
            if (comboCooldownTimer <= 0.0f)
            {
                comboOnCooldown = false;
                comboCount = 0;
                comboMultiplier = 1.0f;
                comboBarVisible = false;
            }
        }

        else if (comboInDecay)
        {
            decayTimer -= deltaTime;
            if (decayTimer <= 0.0f)
            {
                decayTimer = comboDecayRate;
                comboCount = max(comboCount - 1, 0);
                comboMultiplier = 1.0f + (comboCount * 0.02f);

                if (comboCount <= 0)
                {
                    comboInDecay = false;
                    comboOnCooldown = true;
                    comboCooldownTimer = 60.0f;
                }
            }
        }

        else if (comboTimer > 0.0f)
        {
            comboTimer -= deltaTime;
        }

        else if (comboCount > 0 && !comboInDecay)
        {
            comboInDecay = true;
            decayTimer = comboDecayRate;
        }

        float fillPercent = 0.0f;
        if (comboOnCooldown)
        {
            fillPercent = max(0.0f, comboCooldownTimer / 60.0f);
            comboBarFill.setFillColor(sf::Color(100, 100, 100));
        }

        else if (comboInDecay)
        {
            fillPercent = static_cast<float>(comboCount) / static_cast<float>(maxComboCount);
            comboBarFill.setFillColor(sf::Color(255, 120, 120));
        }

        else
        {
            fillPercent = static_cast<float>(comboCount) / static_cast<float>(maxComboCount);
            comboBarFill.setFillColor(sf::Color(100, 200, 255));
        }

        if (comboBarVisible)
        {
            comboBarAlpha = min(comboBarAlpha + fadeSpeed * deltaTime, 255.0f);
        }

        else
        {
            comboBarAlpha = max(comboBarAlpha - fadeSpeed * deltaTime, 0.0f);
        }

        comboBarFill.setSize({ 200.0f * fillPercent, 20.0f });

        for (auto& ripple : ripples)
            ripple.update(deltaTime);

        ripples.erase(remove_if(ripples.begin(), ripples.end(),
            [](const Ripple& r) { return r.isExpired(); }),
            ripples.end());

        for (auto& text : floatingTexts)
            text.update(deltaTime);

        floatingTexts.erase(remove_if(floatingTexts.begin(), floatingTexts.end(),
            [](const FloatingText& t) { return t.isExpired(); }),
            floatingTexts.end());   

        sf::Color bgColor = comboBarBackground.getFillColor();
        sf::Color fillColor = comboBarFill.getFillColor();

        bgColor.a = static_cast<int>(comboBarAlpha);
        fillColor.a = static_cast<int>(comboBarAlpha);

        comboBarBackground.setFillColor(bgColor);
        comboBarFill.setFillColor(fillColor);
    }

    void onClick(float mouseX, float mouseY, sf::Font& font,
        long double baseClickValue,
        long double& bubbles,
        long double& allTimeBubbles,
        long double* allTimeBubblesPerClick,
        bool playEffect = true
    )
    {
        bool isCrit = critDist(rng) < critChance;
        long double finalClickValue = baseClickValue;

        if (!comboOnCooldown)
        {
            comboBarVisible = true;

            if (!comboInDecay)
            {
                comboTimer = comboResetTime;

                if (comboCount < maxComboCount)
                {
                    comboCount++;
                    comboMultiplier = 1.0f + (comboCount * 0.02f);
                }

                if (comboCount >= maxComboCount)
                {
                    comboInDecay = true;
                    decayTimer = comboDecayRate;

                    floatingTexts.emplace_back("Max Combo!", font, sf::Vector2f(mouseX, mouseY));
                    ripples.emplace_back(sf::Vector2f(mouseX, mouseY));
                }
            }

            finalClickValue *= comboMultiplier;
        }

        if (isCrit)
        {
            finalClickValue *= critMultiplier;

            FloatingText critFloating("CRITICAL!", font, { mouseX, mouseY });
            critFloating.baseColor = sf::Color(152, 251, 152);
            critFloating.drawable.setFillColor(critFloating.baseColor);
            critFloating.drawable.setOutlineColor(sf::Color::Black);
            critFloating.drawable.setOutlineThickness(0.5f);

            floatingTexts.emplace_back(critFloating);
        }

        bubbles += finalClickValue;
        allTimeBubbles += finalClickValue;

        if (allTimeBubblesPerClick)
            *allTimeBubblesPerClick += finalClickValue;
    }

    void draw(sf::RenderWindow& window)
    {
        if (comboBarAlpha > 0.0f)
        {
            window.draw(comboBarBackground);
            window.draw(comboBarFill);
        }

        for (auto& ripple : ripples)
            window.draw(ripple.circle);

        for (auto& text : floatingTexts)
            window.draw(text.drawable);
    }

    float getComboMultiplier() const { return comboMultiplier; }
    bool isCoolingDown() const { return comboOnCooldown; }

private:
    int comboCount = 0;
    float comboMultiplier = 1.0f;
    float comboTimer = 0.0f;
    const float comboResetTime = 0.5f;
    const int maxComboCount = 50;

    bool comboInDecay = false;
    float comboDecayRate = 0.25f;
    float decayTimer = 0.0f;

    bool comboOnCooldown = false;
    float comboCooldownTimer = 0.0f;

    bool comboBarVisible = false;

    float comboBarAlpha = 0.0f;
    const float fadeSpeed = 300.0f;

    sf::RectangleShape comboBarBackground;
    sf::RectangleShape comboBarFill;

    vector<FloatingText> floatingTexts;
    vector<Ripple> ripples;

    float critChance = 0.01f;
    float critMultiplier = 3.0f;
    default_random_engine rng;
    uniform_real_distribution<float> critDist{ 0.0f, 1.0f };
};