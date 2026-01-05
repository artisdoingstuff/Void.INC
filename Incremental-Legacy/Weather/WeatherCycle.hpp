#pragma once

#include "Misc/Includes.hpp"
#include "Misc/Popup.hpp"

enum class WeatherType {
    None,
    MarshySprings,
    BubblyRain,
    BubblySun,
    SoapyShower,
    FoamyFog
};

struct WeatherState {
    WeatherType current = WeatherType::None;
    float duration = 0.f;
    sf::Clock timer;
};

inline WeatherState currentWeather;

// --- Weather name display ---
inline string getWeatherName(WeatherType type) {
    switch (type) {
        case WeatherType::MarshySprings:return "Marshy Springs";
        case WeatherType::BubblyRain:   return "Bubbly Rain";
        case WeatherType::BubblySun:    return "Blissful Bubbly Sunshine";
        case WeatherType::SoapyShower:  return "Soapy Shower";
        case WeatherType::FoamyFog:     return "Foamy Fog";
        default: return "Clear Skies";
    }
}

inline string getWeatherEffect(WeatherType type) {
    switch (type) {
        case WeatherType::MarshySprings:return "Marshy Springs roll around the corner!";
        case WeatherType::BubblyRain:   return "It's raining Bubbles!";
        case WeatherType::BubblySun:    return "Blissful rays bounce around!";
        case WeatherType::SoapyShower:  return "Inspiring bubble growth!";
        case WeatherType::FoamyFog:     return "Bubbles are slowing down...";
        default: return "";
    }
}

inline float getWeatherBpsMultiplier(WeatherType type) {
    switch (type) {
        case WeatherType::MarshySprings:return 1.5f;
        case WeatherType::BubblyRain:   return 1.2f;
        case WeatherType::BubblySun:    return 1.15f;
        case WeatherType::SoapyShower:  return 1.1f;
        case WeatherType::FoamyFog:     return 0.9f;
        default: return 1.0f;
    }
}

inline float getWeatherSpawnRateMultiplier(WeatherType type) {
    switch (type) {
        case WeatherType::MarshySprings:return 0.75f;
        case WeatherType::BubblyRain:   return 1.0f;
        case WeatherType::BubblySun:    return 2.0f;
        case WeatherType::SoapyShower:  return 1.0f;
        case WeatherType::FoamyFog:     return 1.5f;
        default: return 1.0f;
    }
}

inline void updateWeather(queue<PopupStruct>& popupQueue)
{
    if (currentWeather.timer.getElapsedTime().asSeconds() >= currentWeather.duration) {
        int roll = rand() % 100;

        if (roll < 45) currentWeather.current = WeatherType::None;
        else if (roll < 55) currentWeather.current = WeatherType::MarshySprings;
        else if (roll < 65) currentWeather.current = WeatherType::SoapyShower;
        else if (roll < 75) currentWeather.current = WeatherType::BubblySun;
        else if (roll < 85) currentWeather.current = WeatherType::BubblyRain;
        else currentWeather.current = WeatherType::FoamyFog;

        currentWeather.duration = 60.f + static_cast<float>(rand() % 60);
        currentWeather.timer.restart();

        popupQueue.push({
            getWeatherName(currentWeather.current) +
            "\n" + getWeatherEffect(currentWeather.current)
            });
    }
}