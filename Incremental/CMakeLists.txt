project(Incremental)

set(CMAKE_CXX_STANDARD 17)

include_directories("C:/SFML-3.0.0+32-Bit/include")

link_directories("C:/SFML-3.0.0+32-Bit/lib")

add_executable (Incremental "Incremental.cpp" "GameFileState.h" "OfflineBubbles.h" "Includes.h" "Upgrades.h" "BubblesFormat.h"  "GlobalBuffHandler.h"    "BubbleMayhem.h" "BubbleFrenzy.h" "BubbleChaos.h" "Bubbles.h" "Achievements.h" "AchievementsList.h" "UpgradesList.h"  "Combo.h" "ShootingStarBubble.h")

target_link_libraries(Incremental sfml-graphics sfml-window sfml-system sfml-audio sfml-network)