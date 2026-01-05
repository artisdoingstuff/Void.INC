project(Incremental-Legacy)

set(CMAKE_CXX_STANDARD 17)

include_directories("C:/SFML-3.0.0+32-Bit/include")

link_directories("C:/SFML-3.0.0+32-Bit/lib")

add_executable (
	Incremental-Legacy "Incremental.cpp" "Misc/GameFileState.hpp" "Bubbles/OfflineBubbles.hpp" "Misc/Includes.hpp" "Upgrades/Upgrades.hpp" "Bubbles/BubblesFormat.hpp"  "Buffs/GlobalBuffHandler.hpp"    "Buffs/Bubbles/BubbleMayhem.hpp" "Buffs/Bubbles/BubbleFrenzy.hpp" "Buffs/Bubbles/BubbleChaos.hpp" "Bubbles/Bubbles.hpp" "Achievements/Achievements.hpp" "Achievements/AchievementsList.hpp" "Upgrades/UpgradesList.hpp"  "Clicking/CritCombo.hpp" "Buffs/Bubbles/ShootingStarBubble.hpp" "Weather/WeatherCycle.hpp" "Misc/Popup.hpp" "Weather/WeatherVisuals.hpp" "Misc/NewsTicker.hpp" "Misc/GlobalVariables.hpp" "Misc/GlobalTextures.hpp" "Marketplace/Trader.hpp"  "Upgrades/UpgradeRarities.hpp" "Ascension/Ascension.hpp" "Misc/GlobalFunctions.hpp" "Buffs/Potions/Potions.hpp" "Buffs/Trader/TradersList.hpp" "Buffs/Potions/PotionsList.hpp" "Marketplace/PotionBrewery.hpp" "Clicking/ClickingHotspots.hpp")

target_link_libraries(Incremental-Legacy sfml-graphics sfml-window sfml-system sfml-audio sfml-network)