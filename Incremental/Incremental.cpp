#include "Achievements.h"
#include "AchievementsList.h"
#include "BubbleFrenzy.h"
#include "BubbleChaos.h"
#include "BubbleMayhem.h"
#include "Bubbles.h"
#include "BubblesFormat.h"
#include "Buffs.h"
// #include "DuckVariants.h"
#include "GameFileState.h"
#include "GlobalBubblesVariants.h"
#include "OfflineBubbles.h"
#include "Upgrades.h"
#include "UpgradesList.h"

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

enum class GameTabs
{
    Items,
    Milestones,
    Achievements
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
int achievementPage = 0;
constexpr int itemsPerPage = 9;

// Global Textures
void loadUpgradeTextures(map<string, sf::Texture>& upgradeTextures)
{
    upgradeTextures["Soap"].loadFromFile("Assets/soap_upgrade.png");
    upgradeTextures["Hand Wash"].loadFromFile("Assets/handwash_upgrade.png");
    upgradeTextures["Shampoo"].loadFromFile("Assets/shampoo_upgrade.png");
    upgradeTextures["Shaving Foam"].loadFromFile("Assets/shaving_foam_upgrade.png");
    upgradeTextures["Toothpaste"].loadFromFile("Assets/toothpaste_upgrade.png");
    upgradeTextures["Loofah"].loadFromFile("Assets/loofah_upgrade.png");
}

sf::Texture bubbleTexture;
sf::Texture globalBubbleTexture;

vector<Achievement> achievements;
vector<UpgradeItem> upgrades;

const sf::Font font("Assets/Fonts/arial.ttf");

string gameVersion = "v1.1.16-beta";

const long double shopInflationMultiplier = 1.15L;

long double bubbles = 0.0L;
long double allTimeBubbles = 0.0L;
long double allTimeBubblesPerClick = 0.0L;

long double bubblesPerSecond = 0.0L;

long double baseBubblesPerClick = 1.0L;
long double clickMultiplier = 1.0L;

long double totalUpgradeCount = 0.0L;

queue<AchievementPopup> popupQueue;
optional<AchievementPopup> currentPopup;

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

    sf::Music bgm;
    bgm.openFromFile("Assets/Audio/GlimpseofEternity_bgm.ogg");
    bgm.setLooping(true);
    bgm.setVolume(50.f);
    bgm.play();

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

    // Achievements Variables
    achievementsList();
    bool isAchievementsAnimating = false;
    float achievementsSlideOffset = -2000.f;

    // Other Variables
    unordered_map<string, float> hoverScales;

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
        upgradeTextures,
        achievements
    );
    displayBubbles = bubbles;

    offlineBubbles(savedTimestamp, bubbles, allTimeBubbles, bubblesPerSecond);

	// Initialize clocks for timing
	sf::Clock secondClock;
	sf::Clock deltaClock;

    sf::Clock shopClock;

    sf::Clock popupTimer;
    sf::Clock achievementsSlideClock;
    sf::Clock itemsSlideClock;
    sf::Clock milestonesSlideClock;

    sf::Clock bubbleChaosClock;
    sf::Clock bubbleChaosSpawnIntervalClock;

    sf::Clock bubbleFrenzyClock;
    sf::Clock bubbleFrenzySpawnIntervalClock;

    sf::Clock bubbleMayhemClock;
    sf::Clock bubbleMayhemSpawnIntervalClock;

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

    sf::Text tooltipText(font);
    tooltipText.setCharacterSize(14);
    tooltipText.setFillColor(sf::Color::White);

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

    constexpr float iconTabSize = UIConstants::TabHeight;

    while (window.isOpen())
    {
        // Bubbles per second buff not showing on display fix
        bubblesPerSecond = 0.0L;
        baseBubblesPerClick = 1.0L;
        clickMultiplier = 1.0L;

        long double realBubblesPerSecond = bubblesPerSecond;
        long double realClickMultiplier = clickMultiplier;
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

        realBubblesPerSecond = bubblesPerSecond * totalMultiplier;

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

            case AchievementType::MilestoneReached:
            {
                int milestoneCount = 0;
                for (const auto& u : upgrades)
                    if (u.isMilestone && u.count > 0)
                        ++milestoneCount;
                achievement.progressValue = min((long double)milestoneCount / achievement.unlockThreshold, 1.0L);
                break;
            }

            default:
                achievement.progressValue = 0.0;
                break;
            }

            if (!achievement.isUnlocked && achievement.progressValue >= 1.0)
            {
                achievement.isUnlocked = true;

                popupQueue.push({ "Achievement Unlocked: " + achievement.name + "\n> " + achievement.description });
                cout << "Achievement unlocked: " << achievement.name << endl;
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

                sf::Vector2f achievementsTabPos = {
                    tabStartPos.x - UIConstants::TabWidth - UIConstants::TabSpacing - iconTabSize - 10.f,
                    tabStartPos.y
                };
                sf::FloatRect achievementsTabRect(achievementsTabPos, { iconTabSize, iconTabSize });

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
                // Achievements Tab
                else if (achievementsTabRect.contains(mousePositionF) && currentTab != GameTabs::Achievements)
                {
                    previousTab = currentTab;
                    currentTab = GameTabs::Achievements;

                    achievementsSlideOffset = -500.f;
                    isAchievementsAnimating = true;
                    achievementsSlideClock.restart();
                    
                    clickHandled = true;
                }
            }

            // Achievements tab pagination
            if (!clickHandled && currentTab == GameTabs::Achievements)
            {
                constexpr int achievementsPerPage = 30;
                constexpr int columns = 6;
                constexpr int rows = achievementsPerPage / columns;
                constexpr float boxWidth = 180.f;
                constexpr float boxHeight = 64.f;
                constexpr float spacingX = 24.f;
                constexpr float spacingY = 16.f;

                float gridStartX = startX - boxWidth * columns - spacingX * (columns - 1);
                float gridStartY = startY;
                float navY = gridStartY + rows * (boxHeight + spacingY) + 10.f;

                sf::Vector2f navSize = { 80.f, 30.f };
                sf::Vector2f prevPos = { gridStartX, navY };
                sf::Vector2f nextPos = { gridStartX + 100.f, navY };

                if (sf::FloatRect(prevPos, navSize).contains(mousePositionF) && achievementPage > 0)
                {
                    achievementPage--;
                    clickHandled = true;
                }
                else if (sf::FloatRect(nextPos, navSize).contains(mousePositionF) &&
                    (achievementPage + 1) * achievementsPerPage < achievements.size())
                {
                    achievementPage++;
                    clickHandled = true;
                }
                else
                {
                    clickHandled = true;
                }
            }

            // Click Bubble
            if (!clickHandled && clickArea.contains(mousePositionF))
            {
                long double clickValue = (baseBubblesPerClick + (bubblesPerSecond * 0.05)) * realClickMultiplier;
                addBubbles(clickValue, bubbles, allTimeBubbles, &allTimeBubblesPerClick, true);
                clickHandled = true;
            }

            // Buffs
            if (!clickHandled)
            {
                handleBubbleClick(activeChaosBubbles, mousePositionF, bubbles, realBubblesPerSecond, bubbleChaosBuffMultiplier, bubblePopping);
                handleBubbleClick(activeFrenzyBubbles, mousePositionF, bubbles, realBubblesPerSecond, bubbleFrenzyBuffMultiplier, bubblePopping);
                handleBubbleClick(activeMayhemBubbles, mousePositionF, bubbles, realBubblesPerSecond, bubbleMayhemBuffMultiplier, bubblePopping);
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
                    sf::Vector2f boxPos = { startX - boxWidth - 20.f, upgradeY };

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

                    sf::Vector2f multibuyStart = { startX - boxWidth - 20.f, upgradeY + 40.f };
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
                    sf::Vector2f prevPos = { startX - boxWidth - 20.f, upgradeY + 85.f };
                    sf::Vector2f nextPos = { startX - boxWidth + 80.f, upgradeY + 85.f };
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
                    box.setOutlineThickness(2.f);
                    box.setOutlineColor(sf::Color(255, 230, 120));
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
                costText.setString(formatDisplayBubbles(upgrade.currentCost));
                costText.setFillColor(sf::Color::Black);
                costText.setPosition({ pos.x + 4.f, pos.y + 4.f });
                window.draw(costText);

                // Draw name
                sf::Text nameText(font);
                nameText.setCharacterSize(12);
                nameText.setString(upgrade.name);
                nameText.setFillColor(sf::Color::Black);
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
                float t = std::min(elapsed / duration, 1.f);
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

                int buyAmount = 1;
                switch (currentMultibuy)
                {
                case MultibuyMode::x1:   buyAmount = 1; break;
                case MultibuyMode::x10:  buyAmount = 10; break;
                case MultibuyMode::x100: buyAmount = 100; break;
                case MultibuyMode::Max:  buyAmount = calculateMaxAffordable(upgrade, bubbles); break;
                }

                long double totalCost = calculateTotalCost(upgrade, buyAmount);
                bool isAffordable = bubbles >= calculateTotalCost(upgrade, buyAmount);

                sf::Vector2f boxSize = { boxWidth, boxHeight };
                sf::Vector2f boxPos = { startX - boxWidth - 20.f + itemsSlideOffset, upgradeY };
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
                    box.setOutlineColor(sf::Color(255, 230, 120));
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
                nameText.setFillColor(sf::Color::Black);
                window.draw(nameText);

                // Cost
                sf::Text costText(font);
                costText.setCharacterSize(14);
                costText.setString(formatDisplayBubbles(totalCost) + " Bubbles");
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

                long double finalItemBps = baseItemBps * totalMultiplier;
                float percent = (realBubblesPerSecond > 0.0)
                    ? static_cast<float>((finalItemBps / realBubblesPerSecond) * 100.0f)
                    : 0.0f;

                int buyAmount = 1;
                switch (currentMultibuy)
                {
                case MultibuyMode::x1:   buyAmount = 1; break;
                case MultibuyMode::x10:  buyAmount = 10; break;
                case MultibuyMode::x100: buyAmount = 100; break;
                case MultibuyMode::Max:  buyAmount = calculateMaxAffordable(*hoveredItem, bubbles); break;
                }

                long double costPreview = calculateTotalCost(*hoveredItem, buyAmount);

                string tooltipStr =
                    hoveredItem->name + "\n" +
                    "BPS: " + formatDisplayBubbles(finalItemBps) + "\n" +
                    "Contribution: " + to_string(percent).substr(0, 5) + "%\n" +
                    "Buy x" + to_string(buyAmount) + " Cost: " + formatDisplayBubbles(costPreview);

                tooltipText.setString(tooltipStr);

                sf::FloatRect bounds = tooltipText.getLocalBounds();
                sf::Vector2f padding = { 10.f, 6.f };
                sf::Vector2f tipSize = bounds.size + padding * 2.f;
                sf::Vector2f tipPos = mousePositionF + sf::Vector2f(12.f, 12.f);

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

                sf::Vector2f multibuyStart = { startX - boxWidth - 20.f + itemsSlideOffset, upgradeY + 40.f };
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

            if (itemPage > 1)
            {
                // Prev/Next Buttons (Rest of Pagination)
                sf::Vector2f prevPos = { startX - boxWidth - 20.f + itemsSlideOffset, upgradeY + 85.f };
                sf::Vector2f nextPos = { startX - boxWidth + 80.f + itemsSlideOffset, upgradeY + 85.f };
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

        else if (currentTab == GameTabs::Achievements)
        {
            float achievementsSlideOffset = 0.f;
            if (isAchievementsAnimating)
            {
                float elapsed = achievementsSlideClock.getElapsedTime().asSeconds();
                float duration = 0.5f;
                float t = min(elapsed / duration, 1.f);
                t = 1.f - pow(1.f - t, 3);
                achievementsSlideOffset = -2000.f * (1.f - t);

                if (t >= 1.f)
                    isAchievementsAnimating = false;
            }

            sf::Vector2f bgSize(1215.f, 400.f);
            sf::Vector2f bgPos(
                (window.getSize().x - bgSize.x) / 2.f + 10.f + achievementsSlideOffset,
                (window.getSize().y - bgSize.y) / 2.f - 147.5f
            );

            sf::RectangleShape bg;
            bg.setSize(bgSize);
            bg.setPosition(bgPos);
            bg.setFillColor(sf::Color(30, 30, 30, 230));
            bg.setOutlineThickness(2.f);
            bg.setOutlineColor(sf::Color(255, 215, 100));
            window.draw(bg);

            constexpr int achievementsPerPage = 30;
            constexpr int columns = 6;
            constexpr int rows = achievementsPerPage / columns;
            constexpr float boxWidth = 180.f;
            constexpr float boxHeight = 64.f;
            constexpr float spacingX = 24.f;
            constexpr float spacingY = 16.f;

            float gridStartX = startX - boxWidth * columns - spacingX * (columns - 1) + achievementsSlideOffset;
            float gridStartY = startY;

            int totalPages = (achievements.size() + achievementsPerPage - 1) / achievementsPerPage;
            achievementPage = clamp(achievementPage, 0, max(0, totalPages - 1));
            int startIdx = achievementPage * achievementsPerPage;
            int endIdx = min<int>(startIdx + achievementsPerPage, achievements.size());

            for (int i = startIdx; i < endIdx; ++i)
            {
                Achievement& a = achievements[i];
                int localIndex = i - startIdx;
                int row = localIndex / columns;
                int col = localIndex % columns;

                sf::Vector2f pos = {
                    gridStartX + col * (boxWidth + spacingX),
                    gridStartY + row * (boxHeight + spacingY)
                };

                sf::RectangleShape box({ boxWidth, boxHeight });
                box.setPosition(pos);
                box.setFillColor(a.isUnlocked ? sf::Color(230, 255, 230) : sf::Color(90, 90, 90));
                window.draw(box);

                if (a.spriteIcon.has_value())
                {
                    sf::Sprite icon = *a.spriteIcon;
                    auto texSize = icon.getTexture().getSize();
                    if (texSize.x > 0 && texSize.y > 0)
                    {
                        icon.setScale({ 32.f / texSize.x, 32.f / texSize.y });
                        icon.setPosition({ pos.x + 8.f, pos.y + 8.f });
                        if (!a.isUnlocked)
                            icon.setColor(sf::Color(80, 80, 80));
                        window.draw(icon);
                    }
                }

                sf::Text nameText(font);
                nameText.setCharacterSize(12);
                nameText.setString(a.name);
                nameText.setFillColor(sf::Color::Black);
                nameText.setPosition({ pos.x + 6.f, pos.y + 6.f });
                window.draw(nameText);

                sf::Text descText(font);
                descText.setCharacterSize(10);
                descText.setString(a.description);
                descText.setFillColor(sf::Color(30, 30, 30));
                descText.setPosition({ pos.x + 6.f, pos.y + 26.f });
                window.draw(descText);

                if (!a.isUnlocked)
                {
                    sf::Text progressText(font);
                    progressText.setCharacterSize(10);

                    float percent = static_cast<float>(a.progressValue * 100.0);

                    progressText.setString(to_string(static_cast<int>(percent)) + "%");
                    progressText.setFillColor(sf::Color::White);
                    progressText.setPosition({ pos.x + boxWidth - 25.f, pos.y + 6.f });
                    window.draw(progressText);
                }
            }

            // Pagination Buttons
            float navY = gridStartY + rows * (boxHeight + spacingY) + 10.f;
            sf::Vector2f navSize = { 80.f, 30.f };
            sf::Vector2f prevPos = { gridStartX, navY };
            sf::Vector2f nextPos = { gridStartX + 100.f, navY };

            sf::RectangleShape prevButton(navSize);
            prevButton.setPosition(prevPos);
            prevButton.setFillColor(achievementPage > 0 ? sf::Color(180, 180, 180) : sf::Color(100, 100, 100));
            window.draw(prevButton);

            sf::Text prevText(font);
            prevText.setCharacterSize(14);
            prevText.setString("Prev");
            prevText.setFillColor(sf::Color::Black);
            prevText.setPosition(prevPos + sf::Vector2f(10.f, 5.f));
            window.draw(prevText);

            sf::RectangleShape nextButton(navSize);
            nextButton.setPosition(nextPos);
            nextButton.setFillColor((achievementPage + 1) < totalPages ? sf::Color(180, 180, 180) : sf::Color(100, 100, 100));
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

        // Achievements Tab (icon-only)
        sf::Vector2f achievementsTabPos = {
            tabStartPos.x - UIConstants::TabWidth - UIConstants::TabSpacing - iconTabSize - 10.f,
            tabStartPos.y
        };

        sf::RectangleShape achievementsTab({ iconTabSize, iconTabSize });
        achievementsTab.setPosition(achievementsTabPos);
        achievementsTab.setFillColor(currentTab == GameTabs::Achievements ? sf::Color::White : sf::Color(150, 150, 150));
        window.draw(achievementsTab);

        sf::Sprite achievementsIcon(globalBubbleTexture);

        sf::Vector2u texSize = globalBubbleTexture.getSize();
        if (texSize.x > 0 && texSize.y > 0)
        {
            float targetSize = iconTabSize * 0.6f;
            float scaleX = targetSize / texSize.x;
            float scaleY = targetSize / texSize.y;
            achievementsIcon.setScale({ scaleX, scaleY });

            achievementsIcon.setPosition({
                achievementsTabPos.x + (iconTabSize - texSize.x * scaleX) / 2.f,
                achievementsTabPos.y + (iconTabSize - texSize.y * scaleY) / 2.f
                });

            window.draw(achievementsIcon);
        }

        // Achievements
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
            AchievementPopup& popup = currentPopup.value();

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
                    realClickMultiplier,
                    realBubblesPerSecond,
                    upgrades,
                    achievements
                );
                window.close();
            }
        }
    }
}