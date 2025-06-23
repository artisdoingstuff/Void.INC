// #include "Achievements.h"
#include "BubbleFrenzy.h"
#include "BubbleChaos.h"
#include "BubbleMayhem.h"
#include "Bubbles.h"
#include "BubblesFormat.h"
#include "Buffs.h"
// #include "DuckVariants.h"
#include "GameFileState.h"
#include "GlobalBubblesVariants.h"
#include "Upgrades.h"
#include "OfflineBubbles.h"

// Upgrade shop variables
namespace UIConstants
{
    constexpr float TabWidth = 150.0f;
    constexpr float TabHeight = 40.0f;
    constexpr float TabSpacing = 10.0f;
    constexpr float TabRightMargin = 40.0f;
    constexpr float TabTopOffset = 50.0f;

    constexpr float StartYOffset = UIConstants::TabTopOffset + UIConstants::TabHeight + 20.0f;
}

float scrollOffset = 0.0f;

enum class UpgradeTab
{
    Items,
    Milestones
};

UpgradeTab currentTab = UpgradeTab::Items;
int itemPage = 0;
constexpr int itemsPerPage = 9;

// Global Textures
map<string, sf::Texture> loadUpgradeTextures()
{
    map<string, sf::Texture> upgradeTextures;

    upgradeTextures["Soap"].loadFromFile("Assets/soap_upgrade.png");
    upgradeTextures["Hand Wash"].loadFromFile("Assets/handwash_upgrade.png");
    upgradeTextures["Shampoo"].loadFromFile("Assets/shampoo_upgrade.png");
    upgradeTextures["Shaving Foam"].loadFromFile("Assets/shaving_foam_upgrade.png");
    upgradeTextures["Toothpaste"].loadFromFile("Assets/toothpaste_upgrade.png");
    upgradeTextures["Loofah"].loadFromFile("Assets/loofah_upgrade.png");
    return upgradeTextures;
}

sf::Texture bubbleTexture;
sf::Texture globalBubbleTexture;

// Global Variables if needed
static map<string, sf::Texture> upgradeTextures = loadUpgradeTextures();

const sf::Font font("Assets/Fonts/arial.ttf");

string gameVersion = "v1.1.2-beta";

const long double shopInflationMultiplier = 1.15L;

long double bubbles = 0.0L;
long double allTimeBubbles = 0.0L;
long double allTimeBubblesPerClick = 0.0L;

long double bubblesPerSecond = 0.0L;

long double baseBubblesPerClick = 1.0L;
long double clickMultiplier = 1.0L;

long double totalUpgradeCount = 0.0L;

// Bubble combo variables
bool isBubbleComboActive = false;
float bubbleComboResetTime = 1.5f;
int currentBubbleCombo = 0;
int highestBubbleCombo = 0;
sf::Clock bubbleComboTimer;

// Global functions
template <typename ActiveBubbleType>
void handleBubbleClick(
    vector<ActiveBubbleType>& bubbleList, const sf::Vector2f& mousePositionF,
    long double& bubbles, float bubbleMultiplier, long double realBubblesPerSecond,
    sf::Sound& bubblePoppingSound, int minMult = 3, int maxMult = 30)
{
    for (auto bubbleIterator = bubbleList.begin(); bubbleIterator != bubbleList.end(); )
    {
        if (bubbleIterator->hitbox.getGlobalBounds().contains(mousePositionF) && !bubbleIterator->isBubblePopping)
        {
            if (!isBubbleComboActive)
            {
                isBubbleComboActive = true;
                bubbleComboTimer.restart();
                currentBubbleCombo = 1;
            }

            else if (bubbleComboTimer.getElapsedTime().asSeconds() <= bubbleComboResetTime)
            {
                currentBubbleCombo++;
                bubbleComboTimer.restart();
            }
            else

            {
                isBubbleComboActive = true;
                currentBubbleCombo = 1;
                bubbleComboTimer.restart();
            }

            long double bubbleComboMultiplier = 1.0 + (static_cast<long double>(currentBubbleCombo - 1) * 0.1L);
            int randomMultiplier = (rand() % (maxMult - minMult)) + minMult;
            
            long double bubbleGain = (realBubblesPerSecond * randomMultiplier * bubbleMultiplier * bubbleComboMultiplier) / 2;
            
            addBubbles(bubbleGain, bubbles, allTimeBubbles);

            bubblePoppingSound.play();
            bubbleIterator->startPoppingBubbles();
        }

        ++bubbleIterator;
    }
}

template <typename SpawnBubbleEvent>
void updateBubbleBuff(
    bool& isBuffActive,
    float buffDuration,
    float spawnInterval,
    sf::Clock& buffClock,
    sf::Clock& spawnClock,
    vector<SpawnBubbleEvent>& activeBubbleList,
    int maxX = 1600,
    int maxY = 900
)
{
    if (!isBuffActive)
        return;

    if (spawnClock.getElapsedTime().asSeconds() >= spawnInterval)
    {
        float x = static_cast<float>(rand() % maxX);
        float y = static_cast<float>(rand() % maxY);
        activeBubbleList.emplace_back(sf::Vector2f(x, y));
        spawnClock.restart();
    }

    if (buffClock.getElapsedTime().asSeconds() > buffDuration)
    {
        isBuffActive = false;
    }
}

template <typename ClearBubbleEvent>
void cleanupExpiredBubbles(vector<ClearBubbleEvent>& bubbles)
{
    bubbles.erase(
        remove_if(
            bubbles.begin(), bubbles.end(),
            [](const ClearBubbleEvent& bubble)
            {
                return bubble.isTimeExpired() || bubble.isPoppingBubblesComplete();
            }
        ),
        bubbles.end()
    );
}

template <typename DrawBubbleEventBubbles>
void updateAndDrawBubbles(vector<DrawBubbleEventBubbles>& bubbleList, sf::RenderWindow& window)
{
    for (auto& bubble : bubbleList)
    {
        if (bubble.isBubblePopping)
            bubble.updatePoppingBubblesAnimation();
        else
            bubble.updateBubbleAlpha();

        window.draw(bubble.bubbleSprite);
    }
}

int main()
{
    globalBubbleBuffVariant currentGlobalBubbleType{};
    duckBuffVariant currentDuckType{};

    sf::RenderWindow window(sf::VideoMode({ 1600, 900 }), "Bubble Incremental | " + gameVersion);
	window.setFramerateLimit(60);
    
    bool isButtonPressed = false;

    time_t savedTimestamp = 0;

	// All textures here
	bubbleTexture.loadFromFile("Assets/bubble.png");
    bubbleTexture.setSmooth(true);

	globalBubbleTexture.loadFromFile("Assets/golden_bubble.png");
    globalBubbleTexture.setSmooth(true);

    // All sounds here
    sf::SoundBuffer rubberDuckQuackBuffer;
    rubberDuckQuackBuffer.loadFromFile("Assets/Audio/rubberDuckQuack.wav");
    sf::Sound rubberDuckQuack(rubberDuckQuackBuffer);

    sf::SoundBuffer bubblePoppingBuffer;
	bubblePoppingBuffer.loadFromFile("Assets/Audio/bubblePopping.wav");
	sf::Sound bubblePopping(bubblePoppingBuffer);

	// Bubbles variables here
    long double displayBubbles = 0.0L;

    long double duckCounter = 0.0L;

    // Buffs variables here
    bool canPressGlobalBubbleBuff = false;

    vector<BubbleChaos> activeChaosBubbles;
    bool isBubbleChaosActive = false;
    float bubbleChaosDuration = 20.0f;
    float bubbleChaosBuffMultiplier = 0.4f;
    float bubbleChaosSpawnInterval = 0.001f;

    vector<BubbleFrenzy> activeFrenzyBubbles;
    bool isBubbleFrenzyActive = false;
    float bubbleFrenzyDuration = 30.0f;
    float bubbleFrenzyBuffMultiplier = 3.0f;
    float bubbleFrenzySpawnInterval = 0.5f;

    vector<BubbleMayhem> activeMayhemBubbles;
    bool isBubbleMayhemActive = false;
    float bubbleMayhemDuration = 20.0f;
    float bubbleMayhemBuffMultiplier = 1.5f;
    float bubbleMayhemSpawnInterval = 0.125f;

    sf::Sprite currentGlobalBubbleSprite(globalBubbleTexture);
    bool isGlobalBubbleBuffActive = false;
    bool doesGlobalBubbleBuffExist = false;
    bool showGlobalBubbleBuffHitbox = false;
    float globalBubbleBuffDuration = 0.0f;
    float globalBubbleBuffMultiplier = 1.0f;
    float globalBubbleBuffSpawnInterval = 120.0f;

    bool isRubberDuckBuffActive = false;
    bool showRubberDuckBuffHitbox = false;
    float rubberDuckBuffDuration = 0.0f;
    float rubberDuckBuffMultiplier = 1.0f;
    float rubberDuckBuffSpawnInterval = 160.0f;

    srand(static_cast<unsigned>(time(0)));

	// Upgrade variables here
    vector<UpgradeItem> upgrades;

    // Items
    upgrades.push_back(
        {
            "Soap",     // Reference Name
            0,          // Item count
            10.0, 10.0, // Item base/current cost
            0.15,        // Item production (bubbles per second)
            10.0,       // Unlock threshold
            false,      // isMilestone
            false,      // unlockedByMilestone
            0.0,        // milestoneTriggerValue
            true,       // isitemUpgrade <-- This is what matters for Items
            false,      // isOtherUpgrade <-- ONLY for non-item upgrades (such as Bubble Milestones)
            true,       // isClickUpgrade <-- True only for Soap
            false,      // isDurationUpgrade <-- Doesn't affect buff duration
            true,       // isMinorUpgrade <-- Will have Minor Upgrades (small buffs)
            true        // isMajorUpgrade <-- Will have Major Upgrades (big buffs)
        }
    );
    upgrades.push_back({ "Hand Wash", 0, 75.0, 75.0, 0.5, 100.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Shampoo", 0, 250.0, 250.0, 1.0, 350.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Shaving Foam", 0, 1000.0, 1000.0, 2.5, 1200.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Toothpaste", 0, 3000.0, 3000.0, 5.0, 3500.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Loofah", 0, 7000.0, 7000.0, 8.0, 7500.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Bubble Bath", 0, 18000.0, 18000.0, 15.0, 20000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Bathtub Jet", 0, 40000.0, 40000.0, 22.0, 50000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Luxury Spa", 0, 100000.0, 100000.0, 35.0, 150000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Foam Pit", 0, 150000.0, 150000.0, 50.0, 200000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Foam Party", 0, 250000.0, 250000.0, 75.0, 500000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Sudsy Soap", 0, 400000.0, 400000.0, 130.0, 750000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Bubble Machine", 0, 750000.0, 750000.0, 200.0, 1000000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Bubbly Pool", 0, 1200000.0, 1200000.0, 300.0, 1500000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Sparkling Water", 0, 1800000.0, 1800000.0, 450.0, 2200000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Carbonated Soda", 0, 2500000.0, 2500000.0, 650.0, 3000000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Bath Bombs", 0, 4000000.0, 4000000.0, 900.0, 5000000.0, false, false, 0.0, true, false, false, false, true, true });
    upgrades.push_back({ "Bubble Wand", 0, 6000000.0, 6000000.0, 1200.0, 7500000.0, false, false, 0.0, true, false, false, false, true, true });

    // Item Upgrades
    generateItemMilestoneUpgrades(upgrades, "Soap", 10.0);
    generateItemMilestoneUpgrades(upgrades, "Hand Wash", 75.0);
    generateItemMilestoneUpgrades(upgrades, "Shampoo", 250.0);
    generateItemMilestoneUpgrades(upgrades, "Shaving Foam", 1000.0);
    generateItemMilestoneUpgrades(upgrades, "Toothpaste", 3000.0);
    generateItemMilestoneUpgrades(upgrades, "Loofah", 7000.0);
    generateItemMilestoneUpgrades(upgrades, "Bubble Bath", 18000.0);
    generateItemMilestoneUpgrades(upgrades, "Bathtub Jet", 40000.0);
    generateItemMilestoneUpgrades(upgrades, "Luxury Spa", 100000.0);
    generateItemMilestoneUpgrades(upgrades, "Foam Pit", 150000.0);
    generateItemMilestoneUpgrades(upgrades, "Foam Party", 250000.0);
    generateItemMilestoneUpgrades(upgrades, "Sudsy Soap", 400000.0);
    generateItemMilestoneUpgrades(upgrades, "Bubble Machine", 750000.0);
    generateItemMilestoneUpgrades(upgrades, "Bubbly Pool", 1200000.0);
    generateItemMilestoneUpgrades(upgrades, "Sparkling Water", 1800000.0);
    generateItemMilestoneUpgrades(upgrades, "Carbonated Soda", 2500000.0);
    generateItemMilestoneUpgrades(upgrades, "Bath Bombs", 4000000.0);
    generateItemMilestoneUpgrades(upgrades, "Bubble Wand", 6000000.0);

    // Other Upgrades
    addOtherMilestoneUpgrade(
        upgrades,       // Upgrade Call
        "Red Bubble",   // Reference Name
        100.0,          // Upgrade Unlock Threshold
        250.0           // Upgrade Cost
    );
    addOtherMilestoneUpgrade(upgrades, "Green Bubble", 250.0, 500.0);
    addOtherMilestoneUpgrade(upgrades, "Blue Bubble", 500.0, 750.0);
    addOtherMilestoneUpgrade(upgrades, "Rubber Ducky", 750.0, 1000.0);
    addOtherMilestoneUpgrade(upgrades, "Rainbow Bubble", 1000.0, 1500.0);
    addOtherMilestoneUpgrade(upgrades, "Cyan Bubble", 1500.0, 2500.0);
    addOtherMilestoneUpgrade(upgrades, "Indigo Bubble", 2500.0, 4000.0);
    addOtherMilestoneUpgrade(upgrades, "Heart-Shaped Bubble", 5000.0, 7500.0);
    addOtherMilestoneUpgrade(upgrades, "Soap Dispenser", 7500.0, 10000.0);
    addOtherMilestoneUpgrade(upgrades, "Bath Bubbles", 10000.0, 15000.0);
    addOtherMilestoneUpgrade(upgrades, "Detergent Bubbles", 15000.0, 20000.0);
    addOtherMilestoneUpgrade(upgrades, "Duck?", 20000.0, 10000.0);
    addOtherMilestoneUpgrade(upgrades, "Carbonation", 25000.0, 35000.0);
    addOtherMilestoneUpgrade(upgrades, "Golden Bubble", 40000.0, 100000.0);
    addOtherMilestoneUpgrade(upgrades, "Hand Wash Refiller", 50000.0, 75000.0);
    addOtherMilestoneUpgrade(upgrades, "Fool's Bubble", 75000.0, 90000.0);
    addOtherMilestoneUpgrade(upgrades, "Misprint Bubble", 100000.0, 250000.0);
    addOtherMilestoneUpgrade(upgrades, "Spring Bubble", 150000.0, 250000.0);
    addOtherMilestoneUpgrade(upgrades, "Cherry Bubble", 200000.0, 300000.0);
    addOtherMilestoneUpgrade(upgrades, "Blossoming Bubble", 250000.0, 350000.0);
    addOtherMilestoneUpgrade(upgrades, "Rose Bubble", 300000.0, 400000.0);
    addOtherMilestoneUpgrade(upgrades, "Dandelion Bubble", 350000.0, 450000.0);
    addOtherMilestoneUpgrade(upgrades, "Charming Bubble", 400000.0, 650000.0);
    addOtherMilestoneUpgrade(upgrades, "Lucky Bubble", 450000.0, 777777.0);
    addOtherMilestoneUpgrade(upgrades, "Sudsy Water Balloon", 500000.0, 850000.0);

    for (auto& upgrade : upgrades)
    {
        auto it = upgradeTextures.find(upgrade.name);
        if (it != upgradeTextures.end())
        {
            upgrade.spriteUpgrade.emplace(it->second);

            if (upgrade.spriteUpgrade.has_value())
                cout << upgrade.name << " has sprite." << endl;
        }
        else
        {
            cerr << "No texture found for " << upgrade.name << endl;
        }
    }

    // Loading game file (if it exists)
    loadFileFromJson(
        savedTimestamp,
        duckCounter,
        bubbles,
        allTimeBubbles,
        allTimeBubblesPerClick,
        baseBubblesPerClick,
        clickMultiplier,
        bubblesPerSecond,
        upgrades,
        upgradeTextures
    );
    displayBubbles = bubbles;

    offlineBubbles(savedTimestamp, bubbles, allTimeBubbles, bubblesPerSecond);

	// Initialize clocks for timing
	sf::Clock secondClock;
	sf::Clock deltaClock;

    sf::Clock bubbleChaosClock;
    sf::Clock bubbleChaosSpawnIntervalClock;

    sf::Clock bubbleFrenzyClock;
    sf::Clock bubbleFrenzySpawnIntervalClock;

    sf::Clock bubbleMayhemClock;
    sf::Clock bubbleMayhemSpawnIntervalClock;

    sf::Clock bubbleBuffClock;
    sf::Clock bubbleBuffSpawnIntervalClock;
    sf::Clock bubbleBuffLifetimeClock;

	sf::Clock globalBubbleBuffClock;
	sf::Clock globalBubbleBuffSpawnIntervalClock;
    sf::Clock globalBubbleBuffLifetimeClock;
    sf::Clock globalBubblePulseClock;

    sf::Clock rubberDuckBuffClock;
	sf::Clock rubberDuckBuffSpawnIntervalClock;
    sf::Clock rubberDuckBuffLifetimeClock;

	// Initialize text objects for displaying bubbles and bubbles per second
	sf::Text bubblesText(font);
    bubblesText.setPosition({ 300, 50 });
    bubblesText.setCharacterSize(24);
	bubblesText.setFillColor(sf::Color::Black);

    sf::Text bubblesPerSecondText(font);
    bubblesPerSecondText.setPosition({ 320, 80 });
    bubblesPerSecondText.setCharacterSize(14);
    bubblesPerSecondText.setFillColor(sf::Color::Black);

	sf::Text bubbleComboText(font);
	bubbleComboText.setPosition({ 800, 120 });
	bubbleComboText.setCharacterSize(24);
	bubbleComboText.setFillColor(sf::Color::Black);

    // Objects for buffs
	sf::RectangleShape globalBubbleBuffHitbox;
    globalBubbleBuffHitbox.setSize(sf::Vector2f(100, 100));
    globalBubbleBuffHitbox.setFillColor(sf::Color::Magenta);
	globalBubbleBuffHitbox.setPosition({ 100, 100 });

    sf::RectangleShape rubberDuckBuffHitbox;
    rubberDuckBuffHitbox.setSize(sf::Vector2f(100, 100));
    rubberDuckBuffHitbox.setFillColor(sf::Color::Red);
	rubberDuckBuffHitbox.setPosition({ -100, -100 });

	// Objects for clicking
    sf::FloatRect clickArea({ 300, 350 }, { 200, 150 });

    sf::RectangleShape clickAreaShape;
    clickAreaShape.setSize(sf::Vector2f(200, 150));
    clickAreaShape.setOutlineColor(sf::Color::Red);
    clickAreaShape.setOutlineThickness(5);
    clickAreaShape.setPosition(sf::Vector2f({ 300, 350 }));

    while (window.isOpen())
    {
        while (const optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                time_t currentTimestamp = time(nullptr);
                saveFileToJson(
                    gameVersion,
                    currentTimestamp,
                    duckCounter,
                    bubbles,
                    allTimeBubbles,
                    allTimeBubblesPerClick,
                    baseBubblesPerClick,
                    clickMultiplier,
                    bubblesPerSecond,
                    upgrades
                );
                window.close();
            }
        }

        // Bubbles per second buff not showing on display fix
        bubblesPerSecond = 0.0L;
        baseBubblesPerClick = 1.0L;
        for (const auto& u : upgrades)
        {
            bubblesPerSecond += getBuffedProduction(u, upgrades);
        }

        long double realBubblesPerSecond = bubblesPerSecond;
        long double realClickMultiplier = clickMultiplier;
        long double realBubbles = bubbles;

        // More upgrade stuff
        map<string, function<void()>> upgradeEffects = {
            { "Red Bubble", [&]() { bubblesPerSecond *= 1.01f; realBubblesPerSecond *= 1.01f; } },
            { "Green Bubble", [&]() { bubblesPerSecond *= 1.01f; realBubblesPerSecond *= 1.01f; } },
            { "Blue Bubble", [&]() { bubblesPerSecond *= 1.01f; realBubblesPerSecond *= 1.01f; } },
            { "Rubber Ducky", [&]() { bubblesPerSecond *= 1.01f; realBubblesPerSecond *= 1.01f; } },
            { "Rainbow Bubble", [&]() { bubblesPerSecond *= 1.02f; realBubblesPerSecond *= 1.02f; } },
            { "Cyan Bubble", [&]() { bubblesPerSecond *= 1.01f; realBubblesPerSecond *= 1.01f; } },
            { "Indigo Bubble", [&]() { bubblesPerSecond *= 1.01f; realBubblesPerSecond *= 1.01f; } },
            { "Heart-Shaped Bubble", [&]() { bubblesPerSecond *= 1.02f; realBubblesPerSecond *= 1.02f; } },
            { "Bath Bubbles", [&]() { bubblesPerSecond *= 1.02f; realBubblesPerSecond *= 1.02f; } },
            { "Detergent Bubbles", [&]() { bubblesPerSecond *= 1.02f; realBubblesPerSecond *= 1.02f; } },
            { "Duck?", [&]() { bubblesPerSecond *= 1.01f; realBubblesPerSecond *= 1.01f; } },
            { "Carbonation", [&]() { bubblesPerSecond *= 1.02f; realBubblesPerSecond *= 1.02f; } },
            { "Golden Bubble", [&]() { bubblesPerSecond *= 1.1f; realBubblesPerSecond *= 1.1f; } },
            { "Fool's Bubble", [&]() { bubblesPerSecond *= 1.01f; realBubblesPerSecond *= 1.01f; } },
            { "Misprint Bubble", [&]() { bubblesPerSecond *= 1.2f; realBubblesPerSecond *= 1.2f; } },
            { "Spring Bubble", [&]() { bubblesPerSecond *= 1.02f; realBubblesPerSecond *= 1.02f; } },
            { "Cherry Bubble", [&]() { bubblesPerSecond *= 1.02f; realBubblesPerSecond *= 1.02f; } },
            { "Blossoming Bubble", [&]() { bubblesPerSecond *= 1.02f; realBubblesPerSecond *= 1.02f; } },
            { "Rose Bubble", [&]() { bubblesPerSecond *= 1.02f; realBubblesPerSecond *= 1.02f; } },
            { "Dandelion Bubble", [&]() { bubblesPerSecond *= 1.02f; realBubblesPerSecond *= 1.02f; } },
            { "Charming Bubble", [&]() { bubblesPerSecond *= 1.02f; realBubblesPerSecond *= 1.02f; } },
            { "Lucky Bubble", [&]() { bubblesPerSecond *= 1.07f; realBubblesPerSecond *= 1.07f; } },
            { "Sudsy Water Balloon", [&]() { bubblesPerSecond *= 1.02f; realBubblesPerSecond *= 1.02f; } }
            //{ "Secret (Placeholder) Bubble", [&]() { realClickMultiplier *= 2.0; bubblesPerSecond *= 1.1; } }
        };

        for (const auto& upgrade : upgrades)
        {
            if (upgrade.count > 0)
            {
                auto it = upgradeEffects.find(upgrade.name);
                if (it != upgradeEffects.end())
                    it->second();
            }
        }

        if (isGlobalBubbleBuffActive)
            realBubblesPerSecond *= globalBubbleBuffMultiplier;

        if (isRubberDuckBuffActive)
            realBubblesPerSecond *= rubberDuckBuffMultiplier;

        // Clicking buffs
        if (isGlobalBubbleBuffActive)
            realClickMultiplier *= globalBubbleBuffMultiplier;

        if (isRubberDuckBuffActive)
            realClickMultiplier *= rubberDuckBuffMultiplier;

        // Clicking stuff
        bool isCurrentlyPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
        bool justClicked = isCurrentlyPressed && !isButtonPressed;

        // Display bubbles and bubbles per second, along with other time logic
        float deltaTime = deltaClock.restart().asSeconds();
        float  smoothingFactor = 1.0f;

		// Get the mouse position relative to the window
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

        if (justClicked)
        {
            // Tab Switching
            sf::Vector2f tabStartPos(
                window.getSize().x - UIConstants::TabWidth - UIConstants::TabRightMargin,
                UIConstants::TabTopOffset
            );

            sf::FloatRect itemsTabRect(
                { tabStartPos.x - UIConstants::TabWidth - UIConstants::TabSpacing, tabStartPos.y },
                { UIConstants::TabWidth, UIConstants::TabHeight }
            );

            sf::FloatRect upgradesTabRect(
                { tabStartPos.x, tabStartPos.y },
                { UIConstants::TabWidth, UIConstants::TabHeight }
            );

            if (itemsTabRect.contains(mousePositionF))
            {
                currentTab = UpgradeTab::Items;
            }
            else if (upgradesTabRect.contains(mousePositionF))
            {
                currentTab = UpgradeTab::Milestones;
            }

            // Click Bubble
            if (clickArea.contains(mousePositionF))
            {
                long double clickValue = (baseBubblesPerClick + (bubblesPerSecond * 0.05)) * realClickMultiplier;
                addBubbles(clickValue, bubbles, allTimeBubbles, &allTimeBubblesPerClick, true);
            }

            // Buff Clicks
            handleBubbleClick(activeChaosBubbles, mousePositionF, bubbles, realBubblesPerSecond, bubbleChaosBuffMultiplier, bubblePopping);
            handleBubbleClick(activeFrenzyBubbles, mousePositionF, bubbles, realBubblesPerSecond, bubbleFrenzyBuffMultiplier, bubblePopping);
            handleBubbleClick(activeMayhemBubbles, mousePositionF, bubbles, realBubblesPerSecond, bubbleMayhemBuffMultiplier, bubblePopping);

            // Upgrade Purchases
            float startX = window.getSize().x - UIConstants::TabWidth - UIConstants::TabRightMargin;
            float startY = UIConstants::TabTopOffset + UIConstants::TabHeight + 20.0f;

            if (currentTab == UpgradeTab::Items)
            {
                constexpr float boxWidth = 350.f;
                constexpr float boxHeight = 60.f;
                constexpr float boxSpacing = 70.f;

                // Collect visible item upgrades
                vector<UpgradeItem*> visibleItems;
                for (auto& upgrade : upgrades)
                {
                    if (!upgrade.isItemUpgrade || upgrade.isMilestone || !upgrade.isUnlocked(allTimeBubbles, upgrades))
                        continue;
                    visibleItems.push_back(&upgrade);
                }

                int totalPages = (visibleItems.size() + itemsPerPage - 1) / itemsPerPage;
                itemPage = std::clamp(itemPage, 0, std::max(0, totalPages - 1));

                int startIdx = itemPage * itemsPerPage;
                int endIdx = std::min<int>(startIdx + itemsPerPage, visibleItems.size());

                float currentY = startY;

                // Item click detection
                for (int i = startIdx; i < endIdx; ++i)
                {
                    UpgradeItem& upgrade = *visibleItems[i];

                    sf::Vector2f boxPos = { startX - boxWidth - 20.f, currentY };
                    sf::Vector2f boxSize = { boxWidth, boxHeight };

                    if (sf::FloatRect(boxPos, boxSize).contains(mousePositionF))
                    {
                        if (upgrade.canAfford(bubbles))
                        {
                            upgrade.purchase(bubbles);
                        }
                    }

                    currentY += boxSpacing;
                }

                // Pagination buttons
                sf::Vector2f prevPos = { startX - boxWidth - 20.f, currentY + 40.f };
                sf::Vector2f nextPos = { startX - boxWidth + 80.f, currentY + 40.f };
                sf::Vector2f buttonSize = { 80.f, 30.f };

                if (sf::FloatRect(prevPos, buttonSize).contains(mousePositionF) && itemPage > 0)
                    itemPage--;

                if (sf::FloatRect(nextPos, buttonSize).contains(mousePositionF) && (itemPage + 1) < totalPages)
                    itemPage++;
            }

            else if (currentTab == UpgradeTab::Milestones)
            {
                constexpr float milestoneSize = 80.f;
                constexpr float spacingX = 50.f;
                constexpr float spacingY = 50.f;
                constexpr int itemsPerRow = 4;

                float milestoneStartX = startX - 350.f;

                int index = 0;
                for (auto& upgrade : upgrades)
                {
                    if ((upgrade.isItemUpgrade && !upgrade.isMilestone) || !upgrade.isUnlocked(allTimeBubbles, upgrades) || upgrade.count >= 1)
                        continue;

                    int row = index / itemsPerRow;
                    int col = index % itemsPerRow;

                    sf::Vector2f pos = {
                        milestoneStartX + col * (milestoneSize + spacingX),
                        startY + row * (milestoneSize + spacingY)
                    };

                    sf::FloatRect milestoneBoxRect(pos, { milestoneSize, milestoneSize });

                    if (milestoneBoxRect.contains(mousePositionF) && upgrade.canAfford(bubbles))
                    {
                        upgrade.purchase(bubbles);
                    }

                    ++index;
                }
            }
        }

        stringstream displayBubblesStream;
        stringstream bubblesPerSecondStream;

        if (realBubblesPerSecond < 10.0f)
        {
            bubblesPerSecondStream << fixed << setprecision(2) << realBubblesPerSecond;
        }

        else if (realBubblesPerSecond < 100.0f)
        {
            bubblesPerSecondStream << fixed << setprecision(1) << realBubblesPerSecond;
		}

        else
        {
            bubblesPerSecondStream << fixed << setprecision(0) << realBubblesPerSecond;
		}

        // Buff logic here
        bool globalBubbleBuffClicked = buffHandler(
            mousePositionF,
            window,

            globalBubbleBuffHitbox,

            globalBubbleBuffClock,
            globalBubbleBuffSpawnIntervalClock,
            globalBubbleBuffLifetimeClock,

            isGlobalBubbleBuffActive,
            showGlobalBubbleBuffHitbox,

            globalBubbleBuffSpawnInterval,
			globalBubbleBuffMultiplier,
            globalBubbleBuffDuration,
            120.0f, 180.0f,

            isCurrentlyPressed,
            isButtonPressed,

            true,

            [&](sf::RectangleShape& buffHitbox, float& buffMultiplier, float& buffDuration)
            {
                selectGlobalBubbleVariant(
                    currentGlobalBubbleType,
                    buffHitbox,
                    buffMultiplier,
                    buffDuration,
                    currentGlobalBubbleSprite
                );
                doesGlobalBubbleBuffExist = true;
                canPressGlobalBubbleBuff = true;
            },
            [&]()
            {
                if (currentGlobalBubbleType.globalBubbleType == globalBubbleVariantType::Normal)
                {
                    doesGlobalBubbleBuffExist = false;
                }

                if (currentGlobalBubbleType.globalBubbleType == globalBubbleVariantType::Multiplicative)
                {
                    doesGlobalBubbleBuffExist = false;
                }

                if (currentGlobalBubbleType.globalBubbleType == globalBubbleVariantType::Additive)
                {
                    float randomValue = 1.0f + static_cast<float>(rand()) / RAND_MAX * 4.0f;
                    bubbles += (1 + pow(10, (floor(log10(realBubbles)) - 2))) * randomValue;
                    doesGlobalBubbleBuffExist = false;
                }

                if (currentGlobalBubbleType.globalBubbleType == globalBubbleVariantType::BubbleChaos)
                {
                    isBubbleChaosActive = true;
                    bubbleChaosClock.restart();
                    bubbleChaosSpawnIntervalClock.restart();
                    doesGlobalBubbleBuffExist = false;
                }

                if (currentGlobalBubbleType.globalBubbleType == globalBubbleVariantType::BubbleFrenzy)
                {
                    isBubbleFrenzyActive = true;
                    bubbleFrenzyClock.restart();
                    bubbleFrenzySpawnIntervalClock.restart();
                    doesGlobalBubbleBuffExist = false;
                }

                if (currentGlobalBubbleType.globalBubbleType == globalBubbleVariantType::BubbleMayhem)
                {
                    isBubbleMayhemActive = true;
                    bubbleMayhemClock.restart();
                    bubbleMayhemSpawnIntervalClock.restart();
                    doesGlobalBubbleBuffExist = false;
                }
            },
            &currentGlobalBubbleSprite,
            true
		);

        if (globalBubbleBuffClicked && canPressGlobalBubbleBuff)
        {
            bubblePopping.play();
            canPressGlobalBubbleBuff = false;
        }

        // Rubber ducks removed for now, will be added back during release

		// Update bubble combo logic
        if (isBubbleComboActive && bubbleComboTimer.getElapsedTime().asSeconds() > bubbleComboResetTime)
        {
            if (currentBubbleCombo > highestBubbleCombo)
                highestBubbleCombo = currentBubbleCombo;

            isBubbleComboActive = false;
            currentBubbleCombo = 0;
        }

		// Bubble chaos, frenzy, and mayhem logic
        updateBubbleBuff(isBubbleChaosActive, bubbleChaosDuration, bubbleChaosSpawnInterval, bubbleChaosClock, bubbleChaosSpawnIntervalClock, activeChaosBubbles);
        updateBubbleBuff(isBubbleFrenzyActive, bubbleFrenzyDuration, bubbleFrenzySpawnInterval, bubbleFrenzyClock, bubbleFrenzySpawnIntervalClock, activeFrenzyBubbles);
        updateBubbleBuff(isBubbleMayhemActive, bubbleMayhemDuration, bubbleMayhemSpawnInterval, bubbleMayhemClock, bubbleMayhemSpawnIntervalClock, activeMayhemBubbles);

		// Remove expired chaos, frenzy, and mayhem bubbles
        cleanupExpiredBubbles(activeChaosBubbles);
        cleanupExpiredBubbles(activeFrenzyBubbles);
        cleanupExpiredBubbles(activeMayhemBubbles);

        // Update bubbles based on time elapsed
        if (secondClock.getElapsedTime().asSeconds() >= 1.0f)
        {
            addBubbles(realBubblesPerSecond, bubbles, allTimeBubbles);
            secondClock.restart();
        }

        displayBubbles += (bubbles - displayBubbles) * smoothingFactor * deltaTime;

        // Shop/Upgrade logic here
        float startX = window.getSize().x - UIConstants::TabWidth - UIConstants::TabRightMargin;
        float startY = UIConstants::TabTopOffset + UIConstants::TabHeight + 20.0f;
        float inputY = startY;

        for (auto& upgrade : upgrades)
        {
            // Skip if not part of active tab
            if (currentTab == UpgradeTab::Items && upgrade.isMilestone)
                continue;
            if (currentTab == UpgradeTab::Milestones && (!upgrade.isMilestone || !upgrade.isUnlocked(allTimeBubbles, upgrades)))
                continue;
            if (!upgrade.isUnlocked(allTimeBubbles, upgrades))
                continue;

            // Define clickable area for upgrade
            sf::FloatRect upgradeClickArea(
                sf::Vector2f(startX - 520.f, inputY),
                sf::Vector2f(500.f, 60.f)
            );

            inputY += 70.f;
        }

        isButtonPressed = isCurrentlyPressed;

        bubblesText.setString(formatDisplayBubbles(displayBubbles) + " Bubbles Formed");
		bubblesPerSecondText.setString(formatDisplayBubbles(realBubblesPerSecond, true) + " Bubbles Per Second");

        window.clear(sf::Color::White);

		window.draw(bubblesText);
		window.draw(bubblesPerSecondText);

        window.draw(clickAreaShape);
        
		// Entire shop/upgrade rendering logic
        float currentY = startY;

        constexpr float boxWidth = 350.f;
        constexpr float boxHeight = 60.f;
        constexpr float boxSpacing = 70.f;
        constexpr float spriteSize = 36.f;

        // Milestone Upgrades
        if (currentTab == UpgradeTab::Milestones)
        {
            float milestoneStartX = startX - 350.f;
            constexpr float milestoneSize = 80.f;
            constexpr float spacingX = 50.f;
            constexpr float spacingY = 50.f;
            constexpr int itemsPerRow = 4;

            int index = 0;
            for (auto& upgrade : upgrades)
            {
                if ((upgrade.isItemUpgrade && !upgrade.isMilestone) || !upgrade.isUnlocked(allTimeBubbles, upgrades) || upgrade.count >= 1)
                    continue;

                int row = index / itemsPerRow;
                int col = index % itemsPerRow;

                sf::Vector2f pos = {
                    milestoneStartX + col * (milestoneSize + spacingX),
                    startY + row * (milestoneSize + spacingY)
                };

                // Milestone box
                sf::RectangleShape box({ milestoneSize, milestoneSize });
                box.setPosition(pos);
                box.setFillColor(upgrade.canAfford(bubbles) ? sf::Color(255, 255, 200) : sf::Color(120, 120, 120));
                window.draw(box);

                // Sprite
                if (upgrade.spriteUpgrade.has_value())
                {
                    sf::Sprite icon = *upgrade.spriteUpgrade;
                    auto texSize = icon.getTexture().getSize();
                    if (texSize.x > 0 && texSize.y > 0)
                    {
                        icon.setScale({ milestoneSize * 0.6f / texSize.x, milestoneSize * 0.6f / texSize.y });
                        icon.setPosition({ pos.x + milestoneSize * 0.2f, pos.y + milestoneSize * 0.2f });
                        window.draw(icon);
                    }
                }

                // Cost
                sf::Text costText(font);
                costText.setCharacterSize(10);
                costText.setString(formatDisplayBubbles(upgrade.currentCost));
                costText.setFillColor(sf::Color::Black);
                costText.setPosition({ pos.x + 4.f, pos.y + 4.f });
                window.draw(costText);

                // Name
                sf::Text nameText(font);
                nameText.setCharacterSize(12);
                nameText.setString(upgrade.name);
                nameText.setFillColor(sf::Color::Black);
                auto bounds = nameText.getLocalBounds();
                nameText.setOrigin({ bounds.position.x + bounds.size.x / 2.f, 0.f });
                nameText.setPosition({ pos.x + milestoneSize / 2.f, pos.y + milestoneSize + 4.f });
                window.draw(nameText);

                ++index;
            }
        }

        else if (currentTab == UpgradeTab::Items)
        {
            vector<UpgradeItem*> visibleItems;
            for (auto& upgrade : upgrades)
            {
                if (!upgrade.isItemUpgrade || upgrade.isMilestone || !upgrade.isUnlocked(allTimeBubbles, upgrades))
                    continue;
                visibleItems.push_back(&upgrade);
            }

            int totalPages = (visibleItems.size() + itemsPerPage - 1) / itemsPerPage;
            itemPage = std::clamp(itemPage, 0, max(0, totalPages - 1));
            int startIdx = itemPage * itemsPerPage;
            int endIdx = min<int>(startIdx + itemsPerPage, visibleItems.size());

            float upgradeY = startY;

            for (int i = startIdx; i < endIdx; ++i)
            {
                UpgradeItem& upgrade = *visibleItems[i];

                sf::RectangleShape box({ boxWidth, boxHeight });
                box.setPosition({ startX - boxWidth - 20.f, upgradeY });
                box.setFillColor(upgrade.canAfford(bubbles) ? sf::Color(220, 255, 220) : sf::Color(140, 140, 140));
                window.draw(box);

                float iconOffsetX = box.getPosition().x + 10.f;
                float iconOffsetY = box.getPosition().y + 20.f;

                if (upgrade.spriteUpgrade.has_value())
                {
                    sf::Sprite icon = *upgrade.spriteUpgrade;
                    auto texSize = icon.getTexture().getSize();
                    if (texSize.x > 0 && texSize.y > 0)
                    {
                        icon.setScale({ spriteSize / texSize.x, spriteSize / texSize.y });
                        icon.setPosition({ iconOffsetX, iconOffsetY });
                        window.draw(icon);
                    }
                }

                // Count
                sf::Text countText(font);
                countText.setCharacterSize(14);
                countText.setString("x" + std::to_string(upgrade.count));
                auto bounds = countText.getLocalBounds();
                sf::Vector2f center = bounds.position + bounds.size / 2.f;
                countText.setOrigin({ center.x, 0.f });
                countText.setPosition({
                    iconOffsetX + spriteSize / 2.f,
                    box.getPosition().y + (upgrade.spriteUpgrade.has_value() ? 2.f : 32.f)
                    });
                countText.setFillColor(sf::Color::Black);
                window.draw(countText);

                // Name
                float nameOffsetX = iconOffsetX + (upgrade.spriteUpgrade.has_value() ? spriteSize + 10.f : 0.f);
                sf::Text nameText(font);
                nameText.setCharacterSize(16);
                nameText.setString(upgrade.name);
                nameText.setPosition({ nameOffsetX, box.getPosition().y + 10.f });
                nameText.setFillColor(sf::Color::Black);
                window.draw(nameText);

                // Cost
                sf::Text costText(font);
                costText.setCharacterSize(14);
                costText.setString(formatDisplayBubbles(upgrade.currentCost) + " Bubbles");
                costText.setPosition({ box.getPosition().x + boxWidth - 140.f, box.getPosition().y + 10.f });
                costText.setFillColor(sf::Color::Black);
                window.draw(costText);

                upgradeY += boxSpacing;
            }

            // Pages
            sf::Text pageText(font);
            pageText.setCharacterSize(14);
            pageText.setString("Page " + to_string(itemPage + 1) + " / " + to_string(max(1, totalPages)));
            pageText.setFillColor(sf::Color::White);
            pageText.setPosition({ startX - boxWidth - 20.f, upgradeY + 10.f });
            window.draw(pageText);

            sf::Vector2f prevPos = { startX - boxWidth - 20.f, upgradeY + 40.f };
            sf::Vector2f nextPos = { startX - boxWidth + 80.f, upgradeY + 40.f };
            sf::Vector2f buttonSize = { 80.f, 30.f };

            sf::RectangleShape prevButton(buttonSize);
            prevButton.setPosition(prevPos);
            prevButton.setFillColor(itemPage > 0 ? sf::Color(180, 180, 180) : sf::Color(100, 100, 100));
            window.draw(prevButton);

            sf::Text prevText(font);
            prevText.setCharacterSize(14);
            prevText.setString("Prev");
            prevText.setFillColor(sf::Color::Black);
            prevText.setPosition(prevPos + sf::Vector2f(10.f, 5.f));
            window.draw(prevText);

            sf::RectangleShape nextButton(buttonSize);
            nextButton.setPosition(nextPos);
            nextButton.setFillColor((itemPage + 1) < totalPages ? sf::Color(180, 180, 180) : sf::Color(100, 100, 100));
            window.draw(nextButton);

            sf::Text nextText(font);
            nextText.setCharacterSize(14);
            nextText.setString("Next");
            nextText.setFillColor(sf::Color::Black);
            nextText.setPosition(nextPos + sf::Vector2f(10.f, 5.f));
            window.draw(nextText);
        }

		// Tab positions and rendering
        sf::Vector2f tabStartPos(
            window.getSize().x - UIConstants::TabWidth - UIConstants::TabRightMargin,
            UIConstants::TabTopOffset
        );

        // Items Tab
        sf::RectangleShape itemsTab(sf::Vector2f(UIConstants::TabWidth, UIConstants::TabHeight));
        itemsTab.setPosition({
            tabStartPos.x - UIConstants::TabWidth - UIConstants::TabSpacing,
            tabStartPos.y
            });
        itemsTab.setFillColor(currentTab == UpgradeTab::Items ? sf::Color::White : sf::Color(150, 150, 150));
        window.draw(itemsTab);

        sf::Text itemsText(font);
        itemsText.setCharacterSize(18);
        itemsText.setString("Items");
        sf::FloatRect itemsBounds = itemsText.getLocalBounds();
        itemsText.setOrigin({
            itemsBounds.position.x + itemsBounds.size.x / 2.f,
            itemsBounds.position.y + itemsBounds.size.y / 2.f
            });
        itemsText.setPosition({
            itemsTab.getPosition().x + UIConstants::TabWidth / 2.f,
            itemsTab.getPosition().y + UIConstants::TabHeight / 2.f
            });
        itemsText.setFillColor(sf::Color::Black);
        window.draw(itemsText);

        // Milestones Tab
        sf::RectangleShape milestonesTab(sf::Vector2f(UIConstants::TabWidth, UIConstants::TabHeight));
        milestonesTab.setPosition(tabStartPos);
        milestonesTab.setFillColor(currentTab == UpgradeTab::Milestones ? sf::Color::White : sf::Color(150, 150, 150));
        window.draw(milestonesTab);

        sf::Text milestonesText(font);
        milestonesText.setCharacterSize(18);
        milestonesText.setString("Upgrades");
        sf::FloatRect milestonesBounds = milestonesText.getLocalBounds();
        milestonesText.setOrigin({
            milestonesBounds.position.x + milestonesBounds.size.x / 2.f,
            milestonesBounds.position.y + milestonesBounds.size.y / 2.f
            });
        milestonesText.setPosition({
            milestonesTab.getPosition().x + UIConstants::TabWidth / 2.f,
            milestonesTab.getPosition().y + UIConstants::TabHeight / 2.f
            });
        milestonesText.setFillColor(sf::Color::Black);
        window.draw(milestonesText);

        updateAndDrawBubbles(activeChaosBubbles, window);
        updateAndDrawBubbles(activeFrenzyBubbles, window);
        updateAndDrawBubbles(activeMayhemBubbles, window);

        if (isBubbleComboActive && currentBubbleCombo > 1)
        {
            bubbleComboText.setString("Bubble Combo x" + to_string(currentBubbleCombo));
            window.draw(bubbleComboText);
        }

        if (showGlobalBubbleBuffHitbox)
        {
            if (!isGlobalBubbleBuffActive)
            {
                float t = globalBubblePulseClock.getElapsedTime().asMilliseconds();
                float pulse = 1.0f + 0.02f * sinf(t * 0.003f);

                currentGlobalBubbleSprite.setScale(sf::Vector2f(pulse * 0.8f, pulse * 0.8f));

                sf::Vector2u textureSize = currentGlobalBubbleSprite.getTexture().getSize();
                currentGlobalBubbleSprite.setOrigin(sf::Vector2f(textureSize.x / 2.0f, textureSize.y / 2.0f));

                sf::Vector2f hitboxCenter = {
                    globalBubbleBuffHitbox.getPosition().x + globalBubbleBuffHitbox.getSize().x / 2.0f,
                    globalBubbleBuffHitbox.getPosition().y + globalBubbleBuffHitbox.getSize().y / 2.0f
                };

                currentGlobalBubbleSprite.setPosition(hitboxCenter);
            }

            window.draw(currentGlobalBubbleSprite);
        }

        if (showRubberDuckBuffHitbox)
        {
            window.draw(rubberDuckBuffHitbox);
        }

        window.display();
    }
}