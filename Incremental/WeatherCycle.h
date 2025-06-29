#pragma once

#include "Includes.h"
#include "Popup.h"

enum class WeatherType {
    None,
    BubblyRain,
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
inline std::string getWeatherName(WeatherType type) {
    switch (type) {
    case WeatherType::BubblyRain:  return "Bubbly Rain";
    case WeatherType::SoapyShower: return "Soapy Shower";
    case WeatherType::FoamyFog:    return "Foamy Fog";
    default: return "Clear Skies";
    }
}

inline std::string getWeatherEffect(WeatherType type) {
    switch (type) {
    case WeatherType::BubblyRain:  return "+20% BPS";
    case WeatherType::SoapyShower: return "+10% BPS";
    case WeatherType::FoamyFog:    return "-10% BPS";
    default: return "No effect";
    }
}

inline float getWeatherBpsMultiplier(WeatherType type) {
    switch (type) {
    case WeatherType::BubblyRain:  return 1.2f;
    case WeatherType::SoapyShower: return 1.1f;
    case WeatherType::FoamyFog:    return 0.9f;
    default: return 1.0f;
    }
}

inline void updateWeather(queue<PopupStruct>& popupQueue)
{
    if (currentWeather.timer.getElapsedTime().asSeconds() >= currentWeather.duration) {
        int roll = rand() % 100;

        if (roll < 45) currentWeather.current = WeatherType::None;
        else if (roll < 65) currentWeather.current = WeatherType::SoapyShower;
        else if (roll < 85) currentWeather.current = WeatherType::BubblyRain;
        else currentWeather.current = WeatherType::FoamyFog;

        currentWeather.duration = 60.f + static_cast<float>(rand() % 60);
        currentWeather.timer.restart();

        popupQueue.push({
            "Current Weather:" + getWeatherName(currentWeather.current) +
            "\n~ " + getWeatherEffect(currentWeather.current)
            });
    }
}

inline void drawWeatherLabel(sf::RenderWindow& window, const sf::Font& font)
{
    sf::Text weatherText(font);
    weatherText.setCharacterSize(14);
    weatherText.setString("Current Weather: " + getWeatherName(currentWeather.current));
    weatherText.setFillColor(sf::Color(200, 230, 255, 0));
    weatherText.setPosition({ window.getSize().x - 210.f, 15.f });
    window.draw(weatherText);
}