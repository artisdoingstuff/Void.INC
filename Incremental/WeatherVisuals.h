#include "Includes.h"
#include "WeatherCycle.h"

struct WeatherParticle {
    sf::CircleShape shape;
    sf::Vector2f velocity;
    float lifetime = 0.f;
    sf::Clock clock;

    WeatherParticle(sf::Vector2f pos, sf::Vector2f vel, float radius, sf::Color color)
        : velocity(vel), lifetime(5.f)
    {
        shape.setRadius(radius);
        shape.setFillColor(color);
        shape.setOrigin({ radius, radius });
        shape.setPosition(pos);
    }

    void update(float dt)
    {
        shape.move(velocity * dt);
        float alpha = 255 * (1.f - clock.getElapsedTime().asSeconds() / lifetime);
        alpha = max(0.f, alpha);
        sf::Color c = shape.getFillColor();
        c.a = static_cast<int>(alpha);
        shape.setFillColor(c);
    }

    bool isExpired() const {
        return clock.getElapsedTime().asSeconds() >= lifetime;
    }
};

inline vector<WeatherParticle> weatherParticles;
inline sf::Clock weatherSpawnClock;

inline void spawnWeatherParticles(sf::Vector2u windowSize)
{
    if (weatherSpawnClock.getElapsedTime().asMilliseconds() < 50)
        return;

    weatherSpawnClock.restart();

    if (currentWeather.current == WeatherType::None) return;

    for (int i = 0; i < 3; ++i)
    {
        float x = static_cast<float>(rand() % windowSize.x);
        float y = static_cast<float>(rand() % 100);

        sf::Color color;
        sf::Vector2f velocity;
        float radius = 2.f;

        switch (currentWeather.current)
        {
        case WeatherType::BubblyRain:
            color = sf::Color(180, 220, 255);
            velocity = { 0.f, 150.f };
            radius = 2.5f;
            break;

        case WeatherType::SoapyShower:
            color = sf::Color(200, 255, 240);
            velocity = { 0.f, 100.f };
            radius = 3.f;
            break;

        case WeatherType::FoamyFog:
            color = sf::Color(230, 230, 230, 100);
            velocity = { 10.f * ((rand() % 3) - 1), 20.f };
            radius = 4.f;
            break;

        default:
            return;
        }

        weatherParticles.emplace_back(sf::Vector2f(x, y), velocity, radius, color);
    }
}

inline void updateAndDrawWeatherParticles(sf::RenderWindow& window, float dt)
{
    spawnWeatherParticles(window.getSize());

    for (auto& p : weatherParticles)
        p.update(dt);

    for (auto& p : weatherParticles)
        window.draw(p.shape);

    weatherParticles.erase(
        remove_if(weatherParticles.begin(), weatherParticles.end(),
            [](const WeatherParticle& p) { return p.isExpired(); }),
        weatherParticles.end()
    );
}