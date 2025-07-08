#pragma once

#include "Includes.h"
#include "GlobalVariables.h"

struct NewsItem {
    string text;
    float durationSec = 60.f;
    bool hasEffect = false;
    function<void()> applyEffect = nullptr;
    function<void()> revertEffect = nullptr;
};

enum class NewsState {
    Idle,
    Entering,
    Displaying,
    Exiting
};

class NewsTicker {
public:
    void addNews(const NewsItem& item) {
        newsPool.push_back(item);
    }

    void loadDefaultNews() {
        newsPool = {
            { "Bubbles swell in the moonlight...", 60.f },
            { "Duckwing Alliance protests price hikes in Scrubtown.", 60.f },
            { "Economists warn: Bubble market is about to burst. Again.", 60.f },
            { "Foam Party ends in sudsy chaos; Mayor \"Regretfully moist.\"", 60.f },
            { "Toothpaste surplus declared 'minty fresh crisis' by officials.", 60.f },
            { "Breaking: Loofah unions demand longer soak times.", 60.f },
            { "Duck scientists discover bubbles are 73% air, 27% mystery.", 60.f },
            { "Carbonated Soda banned in Bubbletown for excessive fizz.", 60.f },
            { "Shampoo prices skyrocket after massive foam spill.", 60.f },
            { "A strange bubble drifts by... then pops with a whisper.", 60.f },
            { "A ducks quack echoes through the foam fields.", 60.f },
            { "A merchant sells golden soap bars at the bazaar.", 60.f },
            { "Soap prices remain stable, economists relieved.", 60.f },
            { "Bubblemancers gather at the Grand Soakening.", 60.f },
            { "Reports of ducks wearing monocles surface again.", 60.f },
            { "Residents warned not to inhale the fizzy mist.", 60.f },
            { "Bubble wizards duel in the center plaza - again.", 60.f },
            { "A quiet hum fills the air. Is that... carbonation?", 60.f },
            { "A retired Duck returns to teach at the academy.", 60.f },
            { "Rain of tiny, harmless bubbles puzzles scientists.", 60.f },
            { "The moon appears unusually glossy tonight.", 60.f },
            { "Festival of Foam attracts record-breaking crowds.", 60.f },
            { "A new spa opens promising \"quantum exfoliation\".", 60.f },
            { "Sudsy Seer predicts \"Bubbly times ahead\".", 60.f },
            { "Pop Idol finalists prepare for the finale concert.", 60.f },
            { "Stray bubble whispers secrets to those who listen.", 60.f },
            { "An orb of unknown origin spotted above Spa Hill.", 60.f },
            { "Bubble density index reaches critical threshold.", 60.f },
            { "Guild of Washers announces new cleansing ritual.", 60.f },
            { "Soap-powered airships pass overhead silently.", 60.f },
            { "Unlicensed soap alchemy reported in Foamtown.", 60.f },
            { "Duck diplomats visit to negotiate bath-time peace.", 60.f },
            { "Bubble tide rises; shoreline scrubbing intensifies.", 60.f },
            { "Old legends speak of the Great Soapstone beneath.", 60.f },
            { "Nighttime brings glowing orbs to the misty marsh.", 60.f },
            { "An alchemist claims to have found dry water.", 60.f },
            { "Bubbles with tiny wings flutter near the willow tree.", 60.f },
            { "Whispers of a seventh bubble element spread quietly.", 60.f },
            {
                "Bubble Tax enacted! Upgrade costs rise by 25%!",
                100.f,
                true,
                []() { globalCostMultiplier *= 1.25f; },
                []() { globalCostMultiplier /= 1.25f; }
            },
            {
                "Bubble Sale! All upgrades temporarily discounted by 10%!",
                20.f,
                true,
                []() { globalCostMultiplier *= 0.9f; },
                []() { globalCostMultiplier /= 0.9f; }
            },
            {
                "An eerie calm settles...",
                120.f,
                true,
                []() { globalBuffSpawnDelayMultiplier *= 2.0f; },
                []() { globalBuffSpawnDelayMultiplier /= 2.0f; }
            }
        };

        static random_device rd;
        static mt19937 g(rd());
        shuffle(newsPool.begin(), newsPool.end(), g);
    }

    void update(float deltaTime) {
        switch (state) {
            case NewsState::Idle:
                loadNext();
                break;

            case NewsState::Entering:
                animationTime += deltaTime;
                positionY = lerp(-80.f, targetY, easeOut(animationTime / animDuration));
                if (animationTime >= animDuration) {
                    positionY = targetY;
                    state = NewsState::Displaying;
                    holdClock.restart();
                }
                break;

            case NewsState::Displaying:
                if (holdClock.getElapsedTime().asSeconds() >= current.durationSec) {
                    state = NewsState::Exiting;
                    animationTime = 0.f;
                }
                break;

            case NewsState::Exiting:
                animationTime += deltaTime;
                positionY = lerp(targetY, -80.f, easeIn(animationTime / animDuration));
                if (animationTime >= animDuration) {
                    positionY = -80.f;
                    if (current.hasEffect && current.revertEffect)
                        current.revertEffect();
                    state = NewsState::Idle;
                }
                break;
        }
    }

    void draw(sf::RenderWindow& window, const sf::Font& font) {
        if (state == NewsState::Idle) return;

        sf::Text text(font);
        text.setCharacterSize(20);
        text.setString(current.text);
        text.setFillColor(current.hasEffect ? sf::Color::Yellow : sf::Color::White);
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(1.f);

        sf::FloatRect bounds = text.getLocalBounds();
        float x = (window.getSize().x - bounds.size.x) / 2.f;
        text.setPosition({ x, positionY });
        window.draw(text);
    }

private:
    vector<NewsItem> newsPool;
    NewsItem current;
    sf::Clock holdClock;
    float positionY = -80.f;
    float targetY = 25.f;
    float animationTime = 0.f;
    float animDuration = 0.4f;

    int index = -1;
    NewsState state = NewsState::Idle;

    void loadNext() {
        if (newsPool.empty()) return;
        index = (index + 1) % newsPool.size();
        current = newsPool[index];

        if (current.hasEffect && current.applyEffect)
            current.applyEffect();

        animationTime = 0.f;
        state = NewsState::Entering;
    }

    static float lerp(float a, float b, float t) {
        return a + (b - a) * t;
    }

    static float easeOut(float t) {
        return 1.f - pow(1.f - t, 3);
    }

    static float easeIn(float t) {
        return pow(t, 3);
    }
};