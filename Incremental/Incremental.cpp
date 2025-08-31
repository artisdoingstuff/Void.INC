#include "Achievements/Achievements.hpp"
#include "Achievements/AchievementsList.hpp"

#include "Bubbles/Bubbles.hpp"
#include "Bubbles/BubblesFormat.hpp"
#include "Bubbles/OfflineBubbles.hpp"

#include "Buffs/Bubbles/BubbleFrenzy.hpp"
#include "Buffs/Bubbles/BubbleChaos.hpp"
#include "Buffs/Bubbles/BubbleMayhem.hpp"
#include "Buffs/Bubbles/MutatedBubble.hpp"
#include "Buffs/Bubbles/ShootingStarBubble.hpp"
#include "Buffs/Trader/TradersList.hpp"
#include "Buffs/GlobalBuffHandler.hpp"

#include "Clicking/ClickingHotspots.hpp"
#include "Clicking/CritCombo.hpp"

#include "Library/AchievementsTab.hpp"
#include "Library/Library.hpp"

#include "Marketplace/Trader.hpp"

#include "Misc/GameFileState.hpp"
#include "Misc/GlobalFunctions.hpp"
#include "Misc/GlobalTextures.hpp"
#include "Misc/GlobalVariables.hpp"
#include "Misc/NewsTicker.hpp"
#include "Misc/Popup.hpp"

#include "Upgrades/Upgrades.hpp"
#include "Upgrades/UpgradesList.hpp"
#include "Upgrades/UpgradeRarities.hpp"

#include "Weather/WeatherCycle.hpp"
#include "Weather/WeatherVisuals.hpp"

// Upgrade shop variables
float scrollOffset = 0.0f;

enum class GameTabs
{
    Items,
    Milestones,
    Library,
    Marketplace
};

GameTabs currentTab = GameTabs::Items;
GameTabs previousTab = GameTabs::Items;


enum class MultibuyMode
{
    x1, x10, x100, Max
};

MultibuyMode currentMultibuy = MultibuyMode::x1;

int itemPage = 0;
int milestonePage = 0;
int traderPage = 0;
constexpr int itemsPerPage = 9;

// Global variables
vector<Achievement> achievements;
vector<UpgradeItem> upgrades;

ComboSystem comboSystem;

sf::Font font("Assets/Fonts/arial.ttf");

string gameVersion = "v1.4.2-beta";

queue<PopupStruct> popupQueue;
optional<PopupStruct> currentPopup;

// Bubble combo variables
bool isBubbleComboActive = false;
float bubbleComboResetTime = 1.5f;
int currentBubbleCombo = 0;
int highestBubbleCombo = 0;
sf::Clock bubbleComboTimer;

// Global functions
void centerText(sf::Text& text, const sf::Vector2f& centerPosition) {
    sf::FloatRect bounds = text.getLocalBounds();

    text.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
        });

    text.setPosition(centerPosition);
}

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
    system("updater.exe");

    sf::RenderWindow window(sf::VideoMode({ 1600, 900 }), "Bubble Incremental | " + gameVersion);
	window.setFramerateLimit(60);

    comboSystem.initializeUI(window.getSize());
    
    bool isButtonPressed = false;

    time_t savedTimestamp = 0;

    // All sounds here
    sf::SoundBuffer rubberDuckQuackBuffer;
    rubberDuckQuackBuffer.loadFromFile("Assets/Audio/rubberDuckQuack.wav");
    sf::Sound rubberDuckQuack(rubberDuckQuackBuffer);

    sf::SoundBuffer bubblePoppingBuffer;
	bubblePoppingBuffer.loadFromFile("Assets/Audio/bubblePopping.wav");
	sf::Sound bubblePopping(bubblePoppingBuffer);

    sf::Music bgm;
    bgm.openFromFile("Assets/Audio/GlimpseofEternity_bgm.ogg");
    bgm.setLooping(true);
    bgm.setVolume(50.f);
    bgm.play();

    // Buffs variables here
    vector<ShootingStar> activeShootingStars;
	bool isShootingStarActive = false;
	float shootingStarDuration = 0.0f;
	float shootingStarBuffMultiplier = 5.0f;

    vector<BubbleChaos> activeChaosBubbles;
    bool isBubbleChaosActive = false;
    float bubbleChaosDuration = 14.0f;
    float bubbleChaosBuffMultiplier = 0.6f;
    float bubbleChaosSpawnInterval = 0.001f;

    vector<BubbleFrenzy> activeFrenzyBubbles;
    bool isBubbleFrenzyActive = false;
    float bubbleFrenzyDuration = 20.0f;
    float bubbleFrenzyBuffMultiplier = 1.5f;
    float bubbleFrenzySpawnInterval = 0.5f;

    vector<BubbleMayhem> activeMayhemBubbles;
    bool isBubbleMayhemActive = false;
    float bubbleMayhemDuration = 20.0f;
    float bubbleMayhemBuffMultiplier = 0.9f;
    float bubbleMayhemSpawnInterval = 0.125f;

    MutatedBubbleBuff mutatedBubble;
    bool isMutatedBubbleActive = false;
    float mutatedBubbleDuration = 0.0f;

    bool isNormalBuffActive = false;
    float normalBuffDuration = 0.0f;
    float normalBuffMultiplier = 2.0f;

    bool isMultiplicativeBuffActive = false;
    float multiplicativeBuffDuration = 0.0f;
    float multiplicativeBuffMultiplier = 3.0f;

	// Weather variables
    currentWeather.current = WeatherType::None;
    currentWeather.duration = 60.f;
    currentWeather.timer.restart();

	// News ticker variables
    NewsTicker newsTicker;
	newsTicker.loadDefaultNews();

    srand(static_cast<unsigned>(time(0)));

    // Upgrade Variables
    upgradesList();
    bool isItemAnimating = false;
    float itemSlideProgress = 1.0f;

    bool isMilestonesAnimating = false;

    map<string, sf::Texture> upgradeTextures;
    loadUpgradeTextures(upgradeTextures);

    for (auto& u : upgrades)
    {
        auto it = upgradeTextures.find(u.name);
        if (it != upgradeTextures.end())
            u.spriteUpgrade = sf::Sprite(it->second);
    }

	// The Library
    Library library(font, achievements);
    bool isLibraryOpen = false;

    // Achievements Variables
    achievementsList();
    AchievementsTab achievementsTab(font);
	map<string, sf::Texture> achievementTextures;
    bool isLibraryAnimating = false;
    float librarySlideOffset = -2000.f;

	loadAchievementTexture(achievementTextures);

    for (auto& achievement : achievements)
    {
        auto it = achievementTextures.find(achievement.name);
        if (it != achievementTextures.end())
            achievement.spriteIcon = sf::Sprite(it->second);
        else
            achievement.spriteIcon = sf::Sprite(achievementTextures["Locked"]);
	}

    // Other Variables
    unordered_map<string, float> hoverScales;

    // Loading game file (if it exists)
    loadFileFromJson(
        savedTimestamp,
        duckCounter,
        buffCounter,
        bubbles,
        allTimeBubbles,
        allTimeBubblesPerClick,
        baseBubblesPerClick,
        clickMultiplier,
        bubblesPerSecond,
        upgrades,
        upgradeTextures,
        achievements
    );
    displayBubbles = bubbles;

    offlineBubbles(savedTimestamp, bubbles, allTimeBubbles, bubblesPerSecond);

	// Initialize clocks for timing
	sf::Clock secondClock;
	sf::Clock deltaClock;

    sf::Clock shopClock;

    sf::Clock traderRestockClock;

    sf::Clock popupTimer;
    sf::Clock librarySlideClock;
    sf::Clock itemsSlideClock;
    sf::Clock milestonesSlideClock;

    sf::Clock bubbleChaosClock;
    sf::Clock bubbleChaosSpawnIntervalClock;

    sf::Clock bubbleFrenzyClock;
    sf::Clock bubbleFrenzySpawnIntervalClock;

    sf::Clock bubbleMayhemClock;
    sf::Clock bubbleMayhemSpawnIntervalClock;

    sf::Clock normalBuffClock;
    sf::Clock multiplicativeBuffClock;

	sf::Clock shootingStarClock;

    sf::Clock mutatedBubbleClock;

    // Objects for buffs
    sf::RectangleShape comboBarBackground;
    comboBarBackground.setSize({ 200.0f, 20.0f });
    comboBarBackground.setPosition({ 20.0f, window.getSize().y - 50.0f });
    comboBarBackground.setFillColor(sf::Color(50, 50, 50));

    sf::RectangleShape comboBarFill;
    comboBarFill.setSize({ 0.0f, 20.0f });
    comboBarFill.setPosition(comboBarBackground.getPosition());
    comboBarFill.setFillColor(sf::Color(100, 200, 255));

	// Objects for clicking
    sf::FloatRect clickArea({ 300, 350 }, { 200, 150 });
    sf::Vector2f clickAreaPosition = { 300.f, 350.f };

    sf::RectangleShape clickAreaShape;
    clickAreaShape.setSize(sf::Vector2f(200, 150));
    clickAreaShape.setOutlineColor(sf::Color::Red);
    clickAreaShape.setOutlineThickness(5);
    clickAreaShape.setPosition(clickAreaPosition);
	sf::Vector2f clickAreaSize = clickAreaShape.getSize();

	// Clicking hotspot variables
    initializeHotspotSystem();
    updateHotspotSystem(clickAreaSize, clickAreaPosition);

    constexpr float iconTabSize = UIConstants::TabHeight;

    // Initialize text objects for displaying bubbles and bubbles per second
    sf::Text bubblesText(font);
    bubblesText.setCharacterSize(24);
    bubblesText.setFillColor(sf::Color::Black);

    sf::Text bubblesPerSecondText(font);
    bubblesPerSecondText.setCharacterSize(14);
    bubblesPerSecondText.setFillColor(sf::Color::Black);

    sf::Text bubbleComboText(font);
    bubbleComboText.setPosition({ 800, 120 });
    bubbleComboText.setCharacterSize(24);
    bubbleComboText.setFillColor(sf::Color::Black);

    sf::Text tooltipText(font);
    tooltipText.setCharacterSize(14);
    tooltipText.setFillColor(sf::Color::White);

    while (window.isOpen())
    {
        // Bubbles per second buff not showing on display fix
        bubblesPerSecond = 0.0L;
        baseBubblesPerClick = 1.0L;
        clickMultiplier = 1.0L;

        long double realBubblesPerSecond = bubblesPerSecond;
        long double realClickMultiplier = clickMultiplier * perkManager.clickMultiplier;
        long double realBubbles = bubbles;

        for (const auto& u : upgrades)
        {
            bubblesPerSecond += getBuffedProduction(u, upgrades);
        }

        // More upgrade stuff
        map<string, function<void()>> upgradeEffects = {
            {
                "Soap Dispenser", [&]() { 
                    if (appliedUpgradeEffects.find("Soap Dispenser") == appliedUpgradeEffects.end()) {
                        perUpgradeMultipliers["Soap"] *= 2.0;
                        appliedUpgradeEffects.insert("Soap Dispenser");
                    }
                }
            },
            {
                "Hand Wash Refiller", [&]() { 
                    if (appliedUpgradeEffects.find("Hand Wash Refiller") == appliedUpgradeEffects.end()) {
                        perUpgradeMultipliers["Hand Wash"] *= 2.0;
                        appliedUpgradeEffects.insert("Hand Wash Refiller");
                    }
                }
            },
            {
                "Lavender Shampoo", [&]() { 
                    if (appliedUpgradeEffects.find("Lavender Shampoo") == appliedUpgradeEffects.end()) {
                        perUpgradeMultipliers["Shampoo"] *= 2.0;
                        appliedUpgradeEffects.insert("Lavender Shampoo");
                    }
                }
            },
            {
                "Green Shaving Foam", [&]() {
                    if (appliedUpgradeEffects.find("Green Shaving Foam") == appliedUpgradeEffects.end()) {
                        perUpgradeMultipliers["Shaving Foam"] *= 2.0;
                        appliedUpgradeEffects.insert("Green Shaving Foam");
                    }
                }
            },
            {
                "More Toothpaste!", [&]() {
                    if (appliedUpgradeEffects.find("More Toothpaste!") == appliedUpgradeEffects.end()) {
                        perUpgradeMultipliers["Toothpaste"] *= 2.0;
                        appliedUpgradeEffects.insert("More Toothpaste!");
                    }
                }
            },
            {
                "Blue Soap Bar", [&]() {
                    if (appliedUpgradeEffects.find("Blue Soap Bar") == appliedUpgradeEffects.end()) {
                        perUpgradeMultipliers["Soap"] *= 2.0;
                        appliedUpgradeEffects.insert("Blue Soap Bar");
                    }
                }
            },
            {
                "Large Loofah", [&]() {
                    if (appliedUpgradeEffects.find("Large Loofah") == appliedUpgradeEffects.end()) {
                        perUpgradeMultipliers["Loofah"] *= 2.0;
                        appliedUpgradeEffects.insert("Large Loofah");
                    }
                }
            },
            {
                "Galaxy Soap", [&]() {
                    if (appliedUpgradeEffects.find("Galaxy Soap") == appliedUpgradeEffects.end()) {
                        perUpgradeMultipliers["Soap"] *= 2.0;
                        appliedUpgradeEffects.insert("Galaxy Soap");
                    }
                }
            },
            {
                "Singularity Sud", [&]() {
                    if (appliedUpgradeEffects.find("Singularity Sud") == appliedUpgradeEffects.end()) {
                        perUpgradeMultipliers["Soap"] *= 2.0;
                        perUpgradeMultipliers["Hand Wash"] *= 2.0;
                        perUpgradeMultipliers["Shampoo"] *= 2.0;
                        appliedUpgradeEffects.insert("Singularity Sud");
                    }
                }
            },
            {
                "Beyond Clean", [&]() {
                    if (appliedUpgradeEffects.find("Beyond Clean") == appliedUpgradeEffects.end()) {
                        perUpgradeMultipliers["Bubble Bath"] *= 2.0;
                        perUpgradeMultipliers["Bathtub Jet"] *= 2.0;
                        appliedUpgradeEffects.insert("Beyond Clean");
                    }
                }
            },
            {
                "True Soap", [&]() {
                    if (appliedUpgradeEffects.find("True Soap") == appliedUpgradeEffects.end()) {
                        perUpgradeMultipliers["Soap"] *= 2.5;
                        appliedUpgradeEffects.insert("True Soap");
                    }
                }
            },
            {
                "Liquid Spectrum", [&]() {
                    if (appliedUpgradeEffects.find("Liquid Spectrum") == appliedUpgradeEffects.end()) {
                        perUpgradeMultipliers["Hand Wash"] *= 2.0;
						perUpgradeMultipliers["Shampoo"] *= 2.0;
                        appliedUpgradeEffects.insert("Liquid Spectrum");
                    }
                }
            },
            {
                "Omnibubble", [&]() {
                    if (appliedUpgradeEffects.find("Omnibubble") == appliedUpgradeEffects.end()) {
                        perUpgradeMultipliers["Soap"] *= 1.2;
						perUpgradeMultipliers["Hand Wash"] *= 1.2;
						perUpgradeMultipliers["Shampoo"] *= 1.2;
						perUpgradeMultipliers["Shaving Foam"] *= 1.2;
						perUpgradeMultipliers["Toothpaste"] *= 1.2;
						perUpgradeMultipliers["Loofah"] *= 1.2;
						perUpgradeMultipliers["Bubble Bath"] *= 1.2;
						perUpgradeMultipliers["Bathtub Jet"] *= 1.2;
						perUpgradeMultipliers["Luxury Spa"] *= 1.2;
						perUpgradeMultipliers["Foam Pit"] *= 1.2;
						perUpgradeMultipliers["Foam Party"] *= 1.2;
						perUpgradeMultipliers["Sudsy Soap"] *= 1.2;
						perUpgradeMultipliers["Bubble Machine"] *= 1.2;
						perUpgradeMultipliers["Bubbly Pool"] *= 1.2;
						perUpgradeMultipliers["Sparkling Water"] *= 1.2;
						perUpgradeMultipliers["Carbonated Soda"] *= 1.2;
						perUpgradeMultipliers["Bath Bombs"] *= 1.2;
						perUpgradeMultipliers["Bubble Wand"] *= 1.2;

                        appliedUpgradeEffects.insert("Omnibubble");
					}
}
            }
        };

        long double totalMultiplier = 1.0;
        for (const auto& u : upgrades)
        {
            if (u.count < 1) continue;

            auto it = globalUpgradeMultiplierValues.find(u.name);
            if (it != globalUpgradeMultiplierValues.end())
                totalMultiplier *= it->second;
        }

        realBubblesPerSecond = bubblesPerSecond * totalMultiplier * perkManager.bpsMultiplier * getWeatherBpsMultiplier(currentWeather.current) * globalBuffMultiplier;

        for (const auto& upgrade : upgrades)
        {
            if (upgrade.count > 0)
            {
                auto it = upgradeEffects.find(upgrade.name);
                if (it != upgradeEffects.end())
                    it->second();
            }
        }

        // Achievements logic here
        for (auto& achievement : achievements)
        {
            // Update progressValue based on type
            switch (achievement.achievementType)
            {
                case AchievementType::TotalBubbles:
                    achievement.progressValue = min(allTimeBubbles / achievement.unlockThreshold, 1.0L);
                    break;

                case AchievementType::Clicks:
                    achievement.progressValue = min(allTimeBubblesPerClick / achievement.unlockThreshold, 1.0L);
                    break;

                case AchievementType::UpgradeCount:
                {
                    long double totalCount = 0.0;
                    for (const auto& u : upgrades)
                        totalCount += u.count;
                    achievement.progressValue = min(totalCount / achievement.unlockThreshold, 1.0L);
                    break;
                }

                case AchievementType::SpecificUpgrade:
                {
                    auto it = find_if(upgrades.begin(), upgrades.end(), [&](const UpgradeItem& u) {
                        return u.name == achievement.targetUpgrade;
                        });
                    if (it != upgrades.end())
                        achievement.progressValue = min(it->count / achievement.unlockThreshold, 1.0L);
                    break;
                }

                case AchievementType::BuffTriggered:
                    achievement.progressValue = min(buffCounter / achievement.unlockThreshold, 1.0L);
                    break;

                default:
                    achievement.progressValue = 0.0;
                    break;
            }

            if (!achievement.isUnlocked && achievement.progressValue >= 1.0)
            {
                achievement.isUnlocked = true;

                PerkEffect effect = getPerkEffectFromAchievementType(achievement.achievementType);
                if (effect.type != AchievementPerkType::None)
                {
                    perkManager.applyPerk(effect.type, effect.value);
                }

                popupQueue.push({ "Achievement Unlocked: " + achievement.name + "\n> " + achievement.description });
                cout << "Achievement unlocked: " << achievement.name << endl;
            }
        }

        // Clicking stuff
        bool isCurrentlyPressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
        bool justClicked = isCurrentlyPressed && !isButtonPressed;

        // Display bubbles and bubbles per second, along with other time logic
        float deltaTime = deltaClock.restart().asSeconds();
        float smoothingFactor = 1.0f;

		// Get the mouse position relative to the window
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f mousePositionF(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

        if (justClicked)
        {
            bool clickHandled = false;

            float startX = window.getSize().x - UIConstants::TabWidth - UIConstants::TabRightMargin;
            float startY = UIConstants::TabTopOffset + UIConstants::TabHeight + 20.0f;

            constexpr float boxWidth = 350.f;
            constexpr float boxHeight = 60.f;
            constexpr float boxSpacing = 70.f;

            // Tab Switching
            if (!clickHandled)
            {
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

                sf::Vector2f libraryTabPos = {
                    tabStartPos.x - UIConstants::TabWidth - UIConstants::TabSpacing - iconTabSize - 10.f,
                    tabStartPos.y
                };
                sf::FloatRect libraryTabRect(libraryTabPos, { iconTabSize, iconTabSize });

                sf::Vector2f marketplaceTabPos = {
                    libraryTabPos.x - iconTabSize - UIConstants::TabSpacing, tabStartPos.y
                };

                sf::FloatRect marketplaceTabRect(marketplaceTabPos, { iconTabSize, iconTabSize });

                // Items Tab
                if (itemsTabRect.contains(mousePositionF) && currentTab != GameTabs::Items)
                {
                    previousTab = currentTab;
                    currentTab = GameTabs::Items;

                    isItemAnimating = true;
                    itemsSlideClock.restart();

                    clickHandled = true;
                }
                // Milestones Tab
                else if (upgradesTabRect.contains(mousePositionF) && currentTab != GameTabs::Milestones)
                {
                    previousTab = currentTab;
                    currentTab = GameTabs::Milestones;

                    isMilestonesAnimating = true;
                    milestonesSlideClock.restart();

                    clickHandled = true;
                }
				// Library Tab
                else if (libraryTabRect.contains(mousePositionF) && currentTab != GameTabs::Library)
                {
                    previousTab = currentTab;
                    currentTab = GameTabs::Library;

                    librarySlideOffset = -500.f;
                    isLibraryAnimating = true;
                    librarySlideClock.restart();

                    clickHandled = true;

                    isLibraryOpen = true;

                    library.startAchievementsAnimation();
                }
                // Marketplace Tab
                else if (marketplaceTabRect.contains(mousePositionF) && currentTab != GameTabs::Marketplace)
                {
                    previousTab = currentTab;
                    currentTab = GameTabs::Marketplace;

                    clickHandled = true;
                }
                // Library disable
                if (currentTab != GameTabs::Library)
                {
                    isLibraryOpen = false;
                }
            }
            
            achievementsTab.handleInput(mousePositionF, !clickHandled, achievements, window, slideOffset);

            // Click Bubble
            if (!clickHandled && clickArea.contains(mousePositionF))
            {
                float hotspotBonus = 1.f;

                if (hotspotActive && currentHotspot.contains(mousePositionF))
                    hotspotBonus = 1.2f;

                long double clickValue = (baseBubblesPerClick + (realBubblesPerSecond * 0.05)) * realClickMultiplier * hotspotBonus;

                comboSystem.onClick(
                    mousePosition.x,
                    mousePosition.y,
                    font,
                    static_cast<float>(clickValue),
                    bubbles,
                    allTimeBubbles,
                    &allTimeBubblesPerClick,
                    true
                );

                clickHandled = true;
            }

            // Buffs
            if (!clickHandled)
            {
                handleBubbleClick(activeChaosBubbles, mousePositionF, bubbles, realBubblesPerSecond, bubbleChaosBuffMultiplier, bubblePopping);
                handleBubbleClick(activeFrenzyBubbles, mousePositionF, bubbles, realBubblesPerSecond, bubbleFrenzyBuffMultiplier, bubblePopping);
                handleBubbleClick(activeMayhemBubbles, mousePositionF, bubbles, realBubblesPerSecond, bubbleMayhemBuffMultiplier, bubblePopping);
                
                for (auto& star : activeShootingStars)
                {
                    if (!star.collected && star.isClicked(mousePositionF))
                    {
                        star.collected = true;
                        clickHandled = true;

						addBubbles((realBubblesPerSecond * 5) * 30, bubbles, allTimeBubbles);

                        bubblePopping.play();
                        break;
                    }
                }
            }

            // Items
            if (!clickHandled && currentTab == GameTabs::Items)
            {
                vector<UpgradeItem*> visibleItems;
                for (auto& upgrade : upgrades)
                {
                    if (!upgrade.isItemUpgrade || upgrade.isMilestone || !upgrade.isUnlocked(allTimeBubbles, upgrades))
                        continue;
                    visibleItems.push_back(&upgrade);
                }

                int totalPages = (visibleItems.size() + itemsPerPage - 1) / itemsPerPage;
                itemPage = clamp(itemPage, 0, max(0, totalPages - 1));
                int startIdx = itemPage * itemsPerPage;
                int endIdx = min<int>(startIdx + itemsPerPage, visibleItems.size());

                float upgradeY = startY;

                // Item clicks
                for (int i = startIdx; i < endIdx; ++i)
                {
                    UpgradeItem& upgrade = *visibleItems[i];
                    sf::Vector2f boxPos = { startX - boxWidth + 150.f, upgradeY };

                    if (sf::FloatRect(boxPos, { boxWidth, boxHeight }).contains(mousePositionF))
                    {
                        int buyAmount = 1;
                        switch (currentMultibuy)
                        {
                            case MultibuyMode::x1:   buyAmount = 1; break;
                            case MultibuyMode::x10:  buyAmount = 10; break;
                            case MultibuyMode::x100: buyAmount = 100; break;
                            case MultibuyMode::Max:  buyAmount = calculateMaxAffordable(upgrade, bubbles); break;
                        }

                        long double totalCost = calculateTotalCost(upgrade, buyAmount);

                        if (bubbles >= totalCost && buyAmount > 0)
                        {
                            if (bubbles < totalCost || buyAmount <= 0)
                                continue;

                            bubbles -= totalCost;
                            upgrade.purchaseRaw(buyAmount);
                        }

                        clickHandled = true;
                        break;
                    }

                    upgradeY += boxSpacing;
                }

                // Multibuy Buttons
                if (!clickHandled)
                {
                    vector<pair<string, MultibuyMode>> multibuyOptions = {
                        { "x1", MultibuyMode::x1 },
                        { "x10", MultibuyMode::x10 },
                        { "x100", MultibuyMode::x100 },
                        { "Max", MultibuyMode::Max }
                    };

                    sf::Vector2f multibuyStart = { startX - boxWidth + 150.f, upgradeY };
                    sf::Vector2f multibuySize = { 60.f, 28.f };

                    for (int i = 0; i < multibuyOptions.size(); ++i)
                    {
                        sf::Vector2f pos = multibuyStart + sf::Vector2f(i * (multibuySize.x + 10.f), 0.f);

                        if (sf::FloatRect(pos, multibuySize).contains(mousePositionF))
                        {
                            currentMultibuy = multibuyOptions[i].second;
                            clickHandled = true;
                            break;
                        }
                    }
                }

                // Pagination
                if (!clickHandled)
                {
                    sf::Vector2f prevPos = { startX - boxWidth + 150.f, upgradeY + 40.f };
                    sf::Vector2f nextPos = { startX - boxWidth + 250.f, upgradeY + 40.f };
                    sf::Vector2f navSize = { 80.f, 30.f };

                    if (sf::FloatRect(prevPos, navSize).contains(mousePositionF) && itemPage > 0)
                    {
                        itemPage--;
                        clickHandled = true;
                    }

                    if (!clickHandled && sf::FloatRect(nextPos, navSize).contains(mousePositionF) && (itemPage + 1) < totalPages)
                    {
                        itemPage++;
                        clickHandled = true;
                    }
                }
            }

            // Milestones
            if (!clickHandled && currentTab == GameTabs::Milestones)
            {
                constexpr float milestoneSize = 80.f;
                constexpr float spacingX = 50.f;
                constexpr float spacingY = 50.f;
                constexpr int itemsPerRow = 4;
                constexpr int itemsPerPage = 20;

                float milestoneStartX = startX - 350.f;

                vector<UpgradeItem*> visibleMilestones;
                for (auto& upgrade : upgrades)
                {
                    if ((upgrade.isItemUpgrade && !upgrade.isMilestone) || !upgrade.isUnlocked(allTimeBubbles, upgrades) || upgrade.count >= 1)
                        continue;

                    visibleMilestones.push_back(&upgrade);
                }

                int totalPages = (visibleMilestones.size() + itemsPerPage - 1) / itemsPerPage;
                milestonePage = clamp(milestonePage, 0, max(0, totalPages - 1));
                int startIdx = milestonePage * itemsPerPage;
                int endIdx = min<int>(startIdx + itemsPerPage, visibleMilestones.size());

                for (int i = startIdx; i < endIdx; ++i)
                {
                    int localIndex = i - startIdx;
                    int row = localIndex / itemsPerRow;
                    int col = localIndex % itemsPerRow;

                    sf::Vector2f pos = {
                        milestoneStartX + col * (milestoneSize + spacingX),
                        startY + row * (milestoneSize + spacingY)
                    };

                    if (sf::FloatRect(pos, { milestoneSize, milestoneSize }).contains(mousePositionF) && visibleMilestones[i]->canAfford(bubbles))
                    {
                        visibleMilestones[i]->purchase(bubbles);
                        clickHandled = true;
                        break;
                    }
                }

                // Milestone Pagination
                if (!clickHandled)
                {
                    int rowsPerPage = (itemsPerPage + itemsPerRow - 1) / itemsPerRow;
                    float navY = startY + rowsPerPage * (milestoneSize + spacingY) + 20.f;

                    sf::Vector2f navSize = { 80.f, 30.f };
                    sf::Vector2f prevPos = { startX - boxWidth - 20.f, navY };
                    sf::Vector2f nextPos = { startX - boxWidth + 80.f, navY };
                    sf::Vector2f buyAllPos = { startX - boxWidth + 200.f, navY };

                    if (sf::FloatRect(prevPos, navSize).contains(mousePositionF) && milestonePage > 0)
                    {
                        milestonePage--;
                        clickHandled = true;
                    }

                    else if (sf::FloatRect(nextPos, navSize).contains(mousePositionF) && (milestonePage + 1) < totalPages)
                    {
                        milestonePage++;
                        clickHandled = true;
                    }

                    else if (sf::FloatRect(buyAllPos, navSize).contains(mousePositionF))
                    {
                        for (int i = startIdx; i < endIdx; ++i)
                        {
                            if (visibleMilestones[i]->canAfford(bubbles))
                                visibleMilestones[i]->purchase(bubbles);
                        }
                        clickHandled = true;
                    }
                }
            }
        };

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
        // Weather Buff
        newsTicker.update(deltaTime);
        updateWeather(popupQueue);
        upgradeTrader(deltaTime);

        if (traderRestockClock.getElapsedTime().asSeconds() >= traderRestockInterval)
        {
            randomlyRestockTraderItems();
            traderRestockClock.restart();
            popupQueue.push({ "Trader has been restocked!" });
        }

        updateHotspotSystem(clickAreaSize, clickAreaPosition);

        for (auto& queued : globalBuffQueue)
        {
            queued.delayRemaining -= deltaTime;
        }

        if (shouldSpawnGlobalBuff())
        {
            sf::Vector2f position = getGlobalBuffSpawnPosition();
            sf::Vector2f size = { 100.0f, 100.0f };

            auto result = selectGlobalBuffVariant(position, size);
            if (result.has_value())
            {
                auto& [index, variant, sprite] = result.value();
                spawnGlobalBuff(sprite, index);
            }

            queueGlobalBuffs(1);
        }

        if (globalBuffQueue.empty())
        {
            queueGlobalBuffs(1);
        }

        updateGlobalBuffs(deltaTime);

        if (justClicked)
        {
            handleGlobalBuffClicks(static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)),
                [&](const GlobalBuffVariant& variant)
                {
                    if (variant.variantType == buffVariantType::globalBubbleBuff)
                        bubblePopping.play();

                    buffCounter++;

                    switch (variant.effectType)
                    {
                        case GlobalBuffType::Normal:
                            isNormalBuffActive = true;
                            normalBuffDuration = variant.duration;
                            normalBuffClock.restart();
                            break;

                        case GlobalBuffType::Multiplicative:
                            isMultiplicativeBuffActive = true;
                            multiplicativeBuffDuration = variant.duration;
                            multiplicativeBuffClock.restart();
                            break;

                        case GlobalBuffType::Additive:
                        {
                            float randomValue = 1.0f + static_cast<float>(rand()) / RAND_MAX * 4.0f;
						    addBubbles((1 + pow(10, (floor(log10(realBubbles)) - 2)))* randomValue, bubbles, allTimeBubbles);
                            break;
                        }

                        case GlobalBuffType::Chaos:
                            isBubbleChaosActive = true;
                            bubbleChaosClock.restart();
                            bubbleChaosSpawnIntervalClock.restart();
                            break;

                        case GlobalBuffType::Frenzy:
                            isBubbleFrenzyActive = true;
                            bubbleFrenzyClock.restart();
                            bubbleFrenzySpawnIntervalClock.restart();
                            break;

                        case GlobalBuffType::Mayhem:
                            isBubbleMayhemActive = true;
                            bubbleMayhemClock.restart();
                            bubbleMayhemSpawnIntervalClock.restart();
                            break;

                        case GlobalBuffType::Mutated:
                        {
                            isMutatedBubbleActive = true;
                            mutatedBubbleDuration = variant.duration;
                            mutatedBubbleClock.restart();

                            mutatedBubble.activate(upgrades);

                            if (!mutatedBubble.backfire && mutatedBubble.affectedItemIndex >= 0)
                            {
                                int percent = static_cast<int>((mutatedBubble.multiplier - 1.f) * 100);
                                string sign = (percent >= 0) ? "+" : "";
                                popupQueue.push({ upgrades[mutatedBubble.affectedItemIndex].name + " buffed: " + sign + std::to_string(percent) + "%", 0.f, 3.f, true });
                            }
                            else
                            {
                                int percent = static_cast<int>((mutatedBubble.multiplier - 1.f) * 100);
                                string sign = (percent >= 0) ? "+" : "";
                                popupQueue.push({ "Mutated Buff Backfire! " + sign + std::to_string(percent) + "%", 0.f, 3.f, true });
                            }

                            break;
                        }

                        case GlobalBuffType::ShootingStar:
                        {
						    isShootingStarActive = true;
                            shootingStarDuration = variant.duration;
                            shootingStarClock.restart();

                            sf::Vector2f start = { 0.0f, 200.0f };
                            sf::Vector2f end = { 1600.0f, 600.0f };
                            float arcHeight = 300.0f;

                            activeShootingStars.emplace_back(shootingStarTexture, start, end, arcHeight, 3.0f);
                            break;
                        }
                    }
                }
            );
        }

        if (isNormalBuffActive)
        {
            if (normalBuffClock.getElapsedTime().asSeconds() >= normalBuffDuration)
                isNormalBuffActive = false;
            else
            {
                realBubblesPerSecond *= normalBuffMultiplier;
                realClickMultiplier *= normalBuffMultiplier;
            }
        }

        if (isMultiplicativeBuffActive)
        {
            if (multiplicativeBuffClock.getElapsedTime().asSeconds() >= multiplicativeBuffDuration)
                isMultiplicativeBuffActive = false;
            else
            {
                realBubblesPerSecond *= multiplicativeBuffMultiplier;
                realClickMultiplier *= multiplicativeBuffMultiplier;
            }
        }

        if (isShootingStarActive)
            if (shootingStarClock.getElapsedTime().asSeconds() >= shootingStarDuration)
                isShootingStarActive = false;
            else
				realBubblesPerSecond *= shootingStarBuffMultiplier;

        if (isBubbleChaosActive)
            if (bubbleChaosClock.getElapsedTime().asSeconds() >= bubbleChaosDuration)
                isBubbleChaosActive = false;
            else
                realBubblesPerSecond *= bubbleChaosBuffMultiplier;

        if (isBubbleFrenzyActive)
            if (bubbleFrenzyClock.getElapsedTime().asSeconds() >= bubbleFrenzyDuration)
                isBubbleFrenzyActive = false;
			else
                realBubblesPerSecond *= bubbleFrenzyBuffMultiplier;

        if (isBubbleMayhemActive)
            if (bubbleMayhemClock.getElapsedTime().asSeconds() >= bubbleMayhemDuration)
				isBubbleMayhemActive = false;
			else
                realBubblesPerSecond *= bubbleMayhemBuffMultiplier;

        if (isMutatedBubbleActive)
        {
            if (mutatedBubbleClock.getElapsedTime().asSeconds() >= mutatedBubbleDuration)
                isMutatedBubbleActive = false;
            else
            {
                float mutatedMultiplier = mutatedBubble.getMultiplier();

                realBubblesPerSecond *= mutatedMultiplier;
                realClickMultiplier *= mutatedMultiplier;
            }
        }


        activeShootingStars.erase(
            remove_if(activeShootingStars.begin(), activeShootingStars.end(),
                [](const ShootingStar& s) { return s.isExpired(); }),
            activeShootingStars.end());

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
        comboSystem.update(deltaTime);

        // Shop/Upgrade logic here
        float startX = window.getSize().x - UIConstants::TabWidth - UIConstants::TabRightMargin;
        float startY = UIConstants::TabTopOffset + UIConstants::TabHeight + 20.0f;
        float inputY = startY;

        for (auto& upgrade : upgrades)
        {
            // Skip if not part of active tab
            if (currentTab == GameTabs::Items && upgrade.isMilestone)
                continue;
            if (currentTab == GameTabs::Milestones && (!upgrade.isMilestone || !upgrade.isUnlocked(allTimeBubbles, upgrades)))
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
        centerText(
            bubblesText,
            {
                clickAreaPosition.x + clickAreaSize.x / 2.f,
                clickAreaPosition.y - 250.f
            }
        );

		bubblesPerSecondText.setString(formatDisplayBubbles(realBubblesPerSecond, true) + " Bubbles Per Second");
        centerText(
            bubblesPerSecondText,
            {
                clickAreaPosition.x + clickAreaSize.x / 2.f,
                clickAreaPosition.y - 230.f
            }
		);

        window.clear(sf::Color::White);

		window.draw(bubblesText);
		window.draw(bubblesPerSecondText);

        window.draw(clickAreaShape);

        // Hotspot rendering
        if (hotspotActive)
        {
            sf::RectangleShape hotspotVisual;
            hotspotVisual.setSize({ currentHotspot.size.x, currentHotspot.size.y });
            hotspotVisual.setPosition({ currentHotspot.position.x, currentHotspot.position.y });

            hotspotVisual.setFillColor(sf::Color(255, 100, 100));
            hotspotVisual.setOutlineColor(sf::Color::Red);
            hotspotVisual.setOutlineThickness(2.f);

            window.draw(hotspotVisual);
        }
        
		// Entire shop/upgrade rendering logic
        float currentY = startY;

        constexpr float boxWidth = 350.f;
        constexpr float boxHeight = 60.f;
        constexpr float boxSpacing = 70.f;
        constexpr float spriteSize = 36.f;

        // Milestone Upgrades
        if (currentTab == GameTabs::Milestones)
        {
            float milestonesSlideOffset = 0.f;
            if (isMilestonesAnimating)
            {
                float elapsed = milestonesSlideClock.getElapsedTime().asSeconds();
                float duration = 0.5f;
                float t = min(elapsed / duration, 1.f);
                t = 1.f - pow(1.f - t, 3);
                milestonesSlideOffset = 500.f * (1.f - t);

                if (t >= 1.f)
                    isMilestonesAnimating = false;
            }

            constexpr float milestoneSize = 80.f;
            constexpr float spacingX = 50.f;
            constexpr float spacingY = 50.f;
            constexpr int itemsPerRow = 4;
            constexpr int itemsPerPage = 20;

            float milestoneStartX = startX - 350.f + milestonesSlideOffset;

            // Collect visible milestone upgrades
            vector<UpgradeItem*> visibleMilestones;
            for (auto& upgrade : upgrades)
            {
                if ((upgrade.isItemUpgrade && !upgrade.isMilestone) || !upgrade.isUnlocked(allTimeBubbles, upgrades) || upgrade.count >= 1)
                    continue;

                visibleMilestones.push_back(&upgrade);
            }

            int totalPages = (visibleMilestones.size() + itemsPerPage - 1) / itemsPerPage;
            milestonePage = clamp(milestonePage, 0, max(0, totalPages - 1));

            int startIdx = milestonePage * itemsPerPage;
            int endIdx = min<int>(startIdx + itemsPerPage, visibleMilestones.size());

            for (int i = startIdx; i < endIdx; ++i)
            {
                int localIndex = i - startIdx;
                int row = localIndex / itemsPerRow;
                int col = localIndex % itemsPerRow;

                sf::Vector2f pos = {
                    milestoneStartX + col * (milestoneSize + spacingX),
                    startY + row * (milestoneSize + spacingY)
                };

                UpgradeItem& upgrade = *visibleMilestones[i];

                // Hover effect
                bool isHovered = sf::FloatRect(pos, { milestoneSize, milestoneSize }).contains(mousePositionF);
                float targetScale = isHovered ? 1.05f : 1.0f;
                float& currentScale = hoverScales[upgrade.name];
                float lerpSpeed = 8.0f * deltaTime;
                currentScale += (targetScale - currentScale) * lerpSpeed;
                currentScale = clamp(currentScale, 1.0f, 1.1f);

                // Draw milestone box
                sf::RectangleShape box({ milestoneSize, milestoneSize });
                box.setOrigin({ box.getSize().x / 2.f, box.getSize().y / 2.f });
                box.setPosition({ pos.x + box.getSize().x / 2.f, pos.y + box.getSize().y / 2.f });
                box.setScale({ currentScale, currentScale });
                box.setFillColor(upgrade.canAfford(bubbles) ? sf::Color(255, 255, 200) : sf::Color(120, 120, 120));

                if (isHovered)
                {
                    sf::Color rarityColor = getRarityColor(upgrade.rarity);
                    if (!upgrade.canAfford(bubbles))
                        rarityColor = sf::Color(rarityColor.r / 2, rarityColor.g / 2, rarityColor.b / 2);

                    box.setOutlineThickness(2.f);
                    box.setOutlineColor(rarityColor);
                }
                else
                {
                    box.setOutlineThickness(0.f);
                }

                window.draw(box);

                // Draw sprite
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

                // Draw cost
                sf::Text costText(font);
                costText.setCharacterSize(10);
                costText.setString(formatDisplayBubbles(upgrade.currentCost * globalCostMultiplier));
                costText.setFillColor(sf::Color::Black);
                costText.setPosition({ pos.x + 4.f, pos.y + 4.f });
                window.draw(costText);

                // Draw name
                sf::Text nameText(font);
                nameText.setCharacterSize(12);
                nameText.setString(upgrade.name);
                nameText.setFillColor(sf::Color::White);
                nameText.setOutlineColor(getRarityColor(upgrade.rarity));
                nameText.setOutlineThickness(2.f);
                auto bounds = nameText.getLocalBounds();
                nameText.setOrigin({ bounds.position.x + bounds.size.x / 2.f, 0.f });
                nameText.setPosition({ pos.x + milestoneSize / 2.f, pos.y + milestoneSize + 4.f });
                window.draw(nameText);
            }

            // Draw pagination
            int rowsPerPage = (itemsPerPage + itemsPerRow - 1) / itemsPerRow;
            float navY = startY + rowsPerPage * (milestoneSize + spacingY) + 20.f;
            sf::Vector2f prevPos = { startX - boxWidth - 20.f + milestonesSlideOffset, navY };
            sf::Vector2f nextPos = { startX - boxWidth + 80.f + milestonesSlideOffset, navY };
            sf::Vector2f buyAllPos = { startX - boxWidth + 200.f + milestonesSlideOffset, navY };
            sf::Vector2f navButtonSize = { 80.f, 30.f };

            if (startIdx < endIdx)
            {
                // Buy All Button
                sf::RectangleShape buyAllButton(navButtonSize);
                buyAllButton.setPosition(buyAllPos);
                buyAllButton.setFillColor(sf::Color(190, 240, 190));
                window.draw(buyAllButton);

                sf::Text buyAllText(font);
                buyAllText.setCharacterSize(14);
                buyAllText.setString("Buy All");
                buyAllText.setFillColor(sf::Color::Black);
                buyAllText.setPosition(buyAllPos + sf::Vector2f(6.f, 5.f));
                window.draw(buyAllText);
            }
            
            if (totalPages > 1)
            {
                // Prev Button
                sf::RectangleShape prevButton(navButtonSize);
                prevButton.setPosition(prevPos);
                prevButton.setFillColor(milestonePage > 0 ? sf::Color(180, 180, 180) : sf::Color(100, 100, 100));
                window.draw(prevButton);

                sf::Text prevText(font);
                prevText.setCharacterSize(14);
                prevText.setString("Prev");
                prevText.setFillColor(sf::Color::Black);
                prevText.setPosition(prevPos + sf::Vector2f(10.f, 5.f));
                window.draw(prevText);

                // Next Button
                sf::RectangleShape nextButton(navButtonSize);
                nextButton.setPosition(nextPos);
                nextButton.setFillColor((milestonePage + 1) < totalPages ? sf::Color(180, 180, 180) : sf::Color(100, 100, 100));
                window.draw(nextButton);

                sf::Text nextText(font);
                nextText.setCharacterSize(14);
                nextText.setString("Next");
                nextText.setFillColor(sf::Color::Black);
                nextText.setPosition(nextPos + sf::Vector2f(10.f, 5.f));
                window.draw(nextText);
            }
        }

        else if (currentTab == GameTabs::Items)
        {
            float itemsSlideOffset = 0.f;
            if (isItemAnimating)
            {
                float elapsed = itemsSlideClock.getElapsedTime().asSeconds();
                float duration = 0.5f;
                float t = min(elapsed / duration, 1.f);
                t = 1.f - pow(1.f - t, 3);

                itemsSlideOffset = 500.f * (1.f - t);
                if (t >= 1.f)
                    isItemAnimating = false;
            }

            vector<UpgradeItem*> visibleItems;
            for (auto& upgrade : upgrades)
            {
                if (!upgrade.isItemUpgrade || upgrade.isMilestone || !upgrade.isUnlocked(allTimeBubbles, upgrades))
                    continue;
                visibleItems.push_back(&upgrade);
            }

            UpgradeItem* hoveredItem = nullptr;

            int totalPages = (visibleItems.size() + itemsPerPage - 1) / itemsPerPage;
            itemPage = clamp(itemPage, 0, max(0, totalPages - 1));
            int startIdx = itemPage * itemsPerPage;
            int endIdx = min<int>(startIdx + itemsPerPage, visibleItems.size());

            float upgradeY = startY;

            // Items
            for (int i = startIdx; i < endIdx; ++i)
            {
                UpgradeItem& upgrade = *visibleItems[i];
                int maxAffordable = calculateMaxAffordable(upgrade, bubbles);

                int buyAmount = 1;
                switch (currentMultibuy)
                {
                    case MultibuyMode::x1:   buyAmount = 1; break;
                    case MultibuyMode::x10:  buyAmount = 10; break;
                    case MultibuyMode::x100: buyAmount = 100; break;
                    case MultibuyMode::Max:  buyAmount = maxAffordable; break;
                }

                long double totalCost = calculateTotalCost(upgrade, buyAmount);
                bool isAffordable = bubbles >= calculateTotalCost(upgrade, buyAmount);

                sf::Vector2f boxSize = { boxWidth, boxHeight };
                sf::Vector2f boxPos = { startX - boxWidth + 150.f + itemsSlideOffset, upgradeY };
                sf::Color baseColor = isAffordable ? sf::Color(200, 255, 200) : sf::Color(140, 140, 140);
                sf::RectangleShape box(boxSize);
                box.setFillColor(baseColor);
                box.setPosition(boxPos);

                bool isHovered = sf::FloatRect(boxPos, boxSize).contains(mousePositionF);

                if (isHovered)
                {
                    hoveredItem = &upgrade;

                    float& currentScale = hoverScales[upgrade.name];
                    float targetScale = 1.05f;

                    float lerpSpeed = 8.0f * deltaTime;
                    currentScale += (targetScale - currentScale) * lerpSpeed;
                    currentScale = clamp(currentScale, 1.0f, 1.1f);

                    box.setOrigin(boxSize * 0.5f);
                    box.setPosition(boxPos + boxSize * 0.5f);
                    box.setScale({ currentScale, currentScale });
                    box.setOutlineThickness(2.f);
                    sf::Color rarityColor = getRarityColor(upgrade.rarity);
                    box.setOutlineColor(rarityColor);
                }

                else
                {
                    float& currentScale = hoverScales[upgrade.name];
                    float lerpSpeed = 8.0f * deltaTime;
                    currentScale += (1.0f - currentScale) * lerpSpeed;

                    box.setOrigin(boxSize * 0.5f);
                    box.setPosition(boxPos + boxSize * 0.5f);
                    box.setScale({ currentScale, currentScale });
                    box.setOutlineThickness(0.f);
                }

                window.draw(box);

                if (sf::FloatRect(boxPos, { boxWidth, boxHeight }).contains(mousePositionF))
                    hoveredItem = &upgrade;

                float iconOffsetX = boxPos.x + 10.f;
                float iconOffsetY = boxPos.y + 20.f;

                if (upgrade.spriteUpgrade.has_value())
                {
                    sf::Sprite icon = *upgrade.spriteUpgrade;
                    auto texSize = icon.getTexture().getSize();
                    if (texSize.x > 0 && texSize.y > 0)
                    {
                        icon.setScale({ spriteSize / texSize.x, spriteSize / texSize.y });

                        icon.setPosition({
                            iconOffsetX,
                            iconOffsetY
                            });

                        window.draw(icon);
                    }
                }

                // Count
                sf::Text countText(font);
                countText.setCharacterSize(14);
                countText.setString("x" + to_string(upgrade.count));
                auto bounds = countText.getLocalBounds();
                sf::Vector2f center = bounds.position + bounds.size / 2.f;
                countText.setOrigin({ center.x, 0.f });
                countText.setPosition({
                    iconOffsetX + spriteSize / 2.f,
                    boxPos.y + (upgrade.spriteUpgrade.has_value() ? 2.f : 32.f)
                    });
                countText.setFillColor(sf::Color::Black);
                window.draw(countText);

                // Name
                float nameOffsetX = iconOffsetX + (upgrade.spriteUpgrade.has_value() ? spriteSize + 10.f : 0.f);
                sf::Text nameText(font);
                nameText.setCharacterSize(16);
                nameText.setString(upgrade.name);
                nameText.setPosition({ nameOffsetX, boxPos.y + 10.f });
                nameText.setFillColor(sf::Color::White);
                nameText.setOutlineColor(getRarityColor(upgrade.rarity));
                nameText.setOutlineThickness(2.f);
                window.draw(nameText);

                // Cost
                sf::Text costText(font);
                costText.setCharacterSize(14);
                string costStr = formatDisplayBubbles(totalCost) + " Bubbles";
                if (currentMultibuy == MultibuyMode::Max)
                    costStr += " (x" + to_string(maxAffordable) + ")";
                costText.setString(costStr);
                costText.setPosition({ boxPos.x + boxWidth - 140.f, boxPos.y + 10.f });
                costText.setFillColor(sf::Color::Black);
                window.draw(costText);

                upgradeY += boxSpacing;
            }

            // Tooltips
            if (hoveredItem)
            {
                long double baseItemBps = getBuffedProduction(*hoveredItem, upgrades);

                long double totalMultiplier = 1.0;
                for (const auto& u : upgrades)
                {
                    if (u.count < 1) continue;
                    auto it = globalUpgradeMultiplierValues.find(u.name);
                    if (it != globalUpgradeMultiplierValues.end())
                        totalMultiplier *= it->second;
                }

                float totalBuffMultiplier = 1.0f;

                if (isNormalBuffActive)
                    totalBuffMultiplier *= normalBuffMultiplier;
                if (isMultiplicativeBuffActive)
                    totalBuffMultiplier *= multiplicativeBuffMultiplier;
				if (isShootingStarActive)
					totalBuffMultiplier *= shootingStarBuffMultiplier;
                if (isBubbleChaosActive)
                    totalBuffMultiplier *= bubbleChaosBuffMultiplier;
                if (isBubbleFrenzyActive)
                    totalBuffMultiplier *= bubbleFrenzyBuffMultiplier;
                if (isBubbleMayhemActive)
                    totalBuffMultiplier *= bubbleMayhemBuffMultiplier;

                long double finalItemBps = baseItemBps * totalMultiplier * perkManager.bpsMultiplier * getWeatherBpsMultiplier(currentWeather.current) * totalBuffMultiplier * globalBuffMultiplier;
                float percent = (realBubblesPerSecond > 0.0)
                    ? static_cast<float>((finalItemBps / realBubblesPerSecond) * 100.0f)
                    : 0.0f;

                int buyAmount = 1;
                switch (currentMultibuy) {
                    case MultibuyMode::x1:   buyAmount = 1; break;
                    case MultibuyMode::x10:  buyAmount = 10; break;
                    case MultibuyMode::x100: buyAmount = 100; break;
                    case MultibuyMode::Max:  buyAmount = calculateMaxAffordable(*hoveredItem, bubbles); break;
                }

                long double costPreview = calculateTotalCost(*hoveredItem, buyAmount);

                string tooltipStr =
                    hoveredItem->name + "\n" +
                    "BPS: " + formatDisplayBubbles(finalItemBps) + "\n" +
                    "Contribution: " + to_string(percent).substr(0, 5) + "%";

                if (!hoveredItem->flavorText.empty()) {
                    tooltipStr += "\n\n\"" + hoveredItem->flavorText + "\"";
                }

                tooltipText.setString(tooltipStr);

                sf::FloatRect bounds = tooltipText.getLocalBounds();
                sf::Vector2f padding = { 10.f, 6.f };
                sf::Vector2f tipSize = bounds.size + padding * 2.f;
                sf::Vector2f tipPos = mousePositionF + sf::Vector2f(12.f, -20.f);

                sf::RectangleShape tipBg(tipSize);
                tipBg.setPosition(tipPos);
                tipBg.setFillColor(sf::Color(30, 30, 30, 220));
                tipBg.setOutlineThickness(1.f);
                tipBg.setOutlineColor(sf::Color(90, 90, 90));

                tooltipText.setPosition(tipPos + padding);
                window.draw(tipBg);
                window.draw(tooltipText);
            }

            if (!visibleItems.empty())
            {
                // Multibuy Buttons
                vector<pair<string, MultibuyMode>> multibuyOptions = {
                    { "x1", MultibuyMode::x1 },
                    { "x10", MultibuyMode::x10 },
                    { "x100", MultibuyMode::x100 },
                    { "Max", MultibuyMode::Max }
                };

                sf::Vector2f multibuyStart = { startX - boxWidth + 150.f + itemsSlideOffset, upgradeY };
                sf::Vector2f multibuySize = { 60.f, 28.f };

                for (int i = 0; i < multibuyOptions.size(); ++i)
                {
                    const auto& [label, mode] = multibuyOptions[i];
                    sf::Vector2f pos = multibuyStart + sf::Vector2f(i * (multibuySize.x + 10.f), 0.f);

                    sf::RectangleShape button(multibuySize);
                    button.setPosition(pos);
                    button.setFillColor(mode == currentMultibuy ? sf::Color(255, 255, 150) : sf::Color(160, 160, 160));
                    window.draw(button);

                    sf::Text text(font);
                    text.setCharacterSize(14);
                    text.setString(label);
                    text.setFillColor(sf::Color::Black);
                    text.setPosition(pos + sf::Vector2f(12.f, 4.f));
                    window.draw(text);

                    if (justClicked && sf::FloatRect(pos, multibuySize).contains(mousePositionF))
                    {
                        currentMultibuy = mode;
                    }
                }
            }

            if (totalPages > 1)
            {
                // Prev/Next Buttons (Rest of Pagination)
                sf::Vector2f prevPos = { startX - boxWidth + 150.f + itemsSlideOffset, upgradeY + 40.f };
                sf::Vector2f nextPos = { startX - boxWidth + 250.f + itemsSlideOffset, upgradeY + 40.f };
                sf::Vector2f navButtonSize = { 80.f, 30.f };

                sf::RectangleShape prevButton(navButtonSize);
                prevButton.setPosition(prevPos);
                prevButton.setFillColor(itemPage > 0 ? sf::Color(180, 180, 180) : sf::Color(100, 100, 100));
                window.draw(prevButton);

                sf::Text prevText(font);
                prevText.setCharacterSize(14);
                prevText.setString("Prev");
                prevText.setFillColor(sf::Color::Black);
                prevText.setPosition(prevPos + sf::Vector2f(10.f, 5.f));
                window.draw(prevText);

                sf::RectangleShape nextButton(navButtonSize);
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
        }

        else if (currentTab == GameTabs::Marketplace)
        {
            renderTraderShop(
				window, font, mousePositionF, justClicked,
                deltaTime, startX, startY, traderPage,
                traderItems, realBubbles, realBubblesPerSecond,
                traderRestockInterval, traderRestockClock, hoverScales);
        }

        else if (currentTab == GameTabs::Library)
        {
            float librarySlideOffset = 0.f;
            if (isLibraryAnimating)
            {
                float elapsed = librarySlideClock.getElapsedTime().asSeconds();
                float duration = 0.5f;
                float t = min(elapsed / duration, 1.f);
                t = 1.f - pow(1.f - t, 3);
                librarySlideOffset = -1500.f * (1.f - t);

                if (t >= 1.f)
                    isLibraryAnimating = false;
            }

            // Background
            sf::Vector2f bgSize(1215.f, 400.f);
            sf::Vector2f bgPos(
                (window.getSize().x - bgSize.x) / 2.f + 10.f + librarySlideOffset,
                (window.getSize().y - bgSize.y) / 2.f - 147.5f
            );

            sf::RectangleShape bg;
            bg.setSize(bgSize);
            bg.setPosition(bgPos);
            bg.setFillColor(sf::Color(30, 30, 30, 230));
            bg.setOutlineThickness(2.f);
            bg.setOutlineColor(sf::Color(255, 215, 100));
            window.draw(bg);
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
        itemsTab.setFillColor(currentTab == GameTabs::Items ? sf::Color::White : sf::Color(150, 150, 150));
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
        milestonesTab.setFillColor(currentTab == GameTabs::Milestones ? sf::Color::White : sf::Color(150, 150, 150));
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

        // Library Tab (icon-only)
        sf::Vector2f libraryTabPos = {
            tabStartPos.x - UIConstants::TabWidth - UIConstants::TabSpacing - iconTabSize - 10.f,
            tabStartPos.y
        };

        sf::RectangleShape libraryTab({ iconTabSize, iconTabSize });
        libraryTab.setPosition(libraryTabPos);
        libraryTab.setFillColor(currentTab == GameTabs::Library ? sf::Color::White : sf::Color(150, 150, 150));
        window.draw(libraryTab);

        sf::Sprite libraryIcon(achievementIconTexture);

        sf::Vector2u libraryTexSize = achievementIconTexture.getSize();
        if (libraryTexSize.x > 0 && libraryTexSize.y > 0)
        {
            float targetSize = iconTabSize * 0.6f;
            float scaleX = targetSize / libraryTexSize.x;
            float scaleY = targetSize / libraryTexSize.y;
            libraryIcon.setScale({ scaleX, scaleY });

            libraryIcon.setPosition({
                libraryTabPos.x + (iconTabSize - libraryTexSize.x * scaleX) / 2.f,
                libraryTabPos.y + (iconTabSize - libraryTexSize.y * scaleY) / 2.f
                });

            window.draw(libraryIcon);
        }

        // Marketplace Tab (icon-only)
        sf::Vector2f marketplaceTabPos = {
            tabStartPos.x - UIConstants::TabWidth - UIConstants::TabSpacing - iconTabSize * 2.f - 20.f,
            tabStartPos.y
        };

        sf::RectangleShape marketplaceTab({ iconTabSize, iconTabSize });
        marketplaceTab.setPosition(marketplaceTabPos);
        marketplaceTab.setFillColor(currentTab == GameTabs::Marketplace ? sf::Color::White : sf::Color(150, 150, 150));
        window.draw(marketplaceTab);

        sf::Sprite marketplaceIcon(marketplaceIconTexture);
        sf::Vector2u marketplaceTexSize = marketplaceIconTexture.getSize();

        if (marketplaceTexSize.x > 0 && marketplaceTexSize.y > 0)
        {
            float targetSize = iconTabSize * 0.6f;
            float scaleX = targetSize / marketplaceTexSize.x;
            float scaleY = targetSize / marketplaceTexSize.y;
            marketplaceIcon.setScale({ scaleX, scaleY });

            marketplaceIcon.setPosition({
                marketplaceTabPos.x + (iconTabSize - marketplaceTexSize.x * scaleX) / 2.f,
                marketplaceTabPos.y + (iconTabSize - marketplaceTexSize.y * scaleY) / 2.f
                });

            window.draw(marketplaceIcon);
        }

        // Library
        if (isLibraryOpen) {
            library.handleInput(mousePositionF, justClicked, window);
            library.draw(window);
        }

        // Popups
        if (currentPopup.has_value())
        {
            currentPopup->elapsed += deltaTime;

            if (currentPopup->elapsed >= currentPopup->duration)
            {
                currentPopup.reset();
            }
        }
        else if (!popupQueue.empty())
        {
            currentPopup = popupQueue.front();
            popupQueue.pop();
            popupTimer.restart();
        }

        if (currentPopup.has_value())
        {
            PopupStruct& popup = currentPopup.value();

            const float slideDuration = 0.4f;
            const float maxOffsetX = 380.f;

            float windowHeight = window.getSize().y;
            float baseY = windowHeight - 80.f;

            float xOffset = 0.f;
            if (popup.elapsed < slideDuration)
            {
                float t = popup.elapsed / slideDuration;
                xOffset = maxOffsetX * (1.f - pow(1.f - t, 3));
            }
            else if (popup.elapsed > popup.duration - slideDuration)
            {
                float t = (popup.elapsed - (popup.duration - slideDuration)) / slideDuration;
                xOffset = maxOffsetX * (1.f - pow(t, 3));
            }
            else
            {
                xOffset = maxOffsetX;
            }

            sf::Vector2f popupPos = { -maxOffsetX + xOffset + 20.f, baseY };

            sf::Vector2f popupSize = { 360.f, 50.f };
            sf::RectangleShape popupBg(popupSize);
            popupBg.setPosition(popupPos);
            popupBg.setFillColor(sf::Color(30, 30, 30, 230));
            popupBg.setOutlineThickness(2.f);
            popupBg.setOutlineColor(sf::Color(255, 215, 100));

            sf::Text popupText(font);
            popupText.setString(popup.title);
            popupText.setCharacterSize(14);
            popupText.setFillColor(sf::Color::White);
            popupText.setPosition(popupPos + sf::Vector2f(10.f, 6.f));

            window.draw(popupBg);
            window.draw(popupText);
        }

		// Hi buff related rendering

        if (isBubbleComboActive && currentBubbleCombo > 1)
        {
            bubbleComboText.setString("Bubble Combo x" + to_string(currentBubbleCombo));
            window.draw(bubbleComboText);
        }

        drawGlobalBuffs(window);

        updateAndDrawBubbles(activeChaosBubbles, window);
        updateAndDrawBubbles(activeFrenzyBubbles, window);
        updateAndDrawBubbles(activeMayhemBubbles, window);

        comboSystem.draw(window);

        updateAndDrawWeatherParticles(window, deltaTime);

        newsTicker.draw(window, font);

        for (auto& star : activeShootingStars)
        {
            star.update();

            if (star.sprite.has_value())
                window.draw(star.sprite.value());
        }

        window.display();

        while (const optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
				// Disable all buffs on exit
                if (isNormalBuffActive)
                    realBubblesPerSecond /= normalBuffMultiplier;
				if (isMultiplicativeBuffActive)
					realBubblesPerSecond /= multiplicativeBuffMultiplier;
                if (isShootingStarActive)
					realBubblesPerSecond /= shootingStarBuffMultiplier;
                if (isBubbleChaosActive)
					realBubblesPerSecond /= bubbleChaosBuffMultiplier;
				if (isBubbleFrenzyActive)
					realBubblesPerSecond /= bubbleFrenzyBuffMultiplier;
				if (isBubbleMayhemActive)
					realBubblesPerSecond /= bubbleMayhemBuffMultiplier;

                time_t currentTimestamp = time(nullptr);
                saveFileToJson(
                    currentTimestamp,
                    duckCounter,
                    buffCounter,
                    bubbles,
                    allTimeBubbles,
                    allTimeBubblesPerClick,
                    baseBubblesPerClick,
                    realClickMultiplier,
                    realBubblesPerSecond / getWeatherBpsMultiplier(currentWeather.current) / globalBuffMultiplier,
                    upgrades,
                    achievements
                );
                versionSave(gameVersion);
                window.close();
            }
        }
    }
}