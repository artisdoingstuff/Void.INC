#pragma once
#include "GIncludes.hpp"

// Global references
inline std::string voidVersion = "v1.0.1"; // Version control for the game (Updater purposes)

inline bool initialization = false;

enum class InitState {
    IDLE,
    VORTEX_EXPANSION,
    VORTEX_SHRINK,
    LOADING_BAR,
    ROOTDIR
};

inline InitState currentInitStep = InitState::IDLE;
inline float loadingProgress = 0.0f;
float initTimer = 0.0f;

inline bool canClick = true; // Pre-initialization
inline bool canClickInit = false; // Post-reinitialization

enum class Directory { NONE, LOGIC_GATES, HOTFIXES, REINIT };
inline Directory currentDir = Directory::LOGIC_GATES;

inline sf::FloatRect logicFolderBounds;
inline sf::FloatRect hotfixFolderBounds;
inline sf::FloatRect reinitFolderBounds;

// Dynamic shenanigans
inline float costMult = 1.0f; // Change the cost of all logicGate

// Inflation multipliers
inline float logicGateInflation = 1.135f; // Inflates the cost of logicGate (base*(1.135^amt))

// Important data
inline long double bits = 0.0L; // Current bits
inline long double allBits = 0.0L; // Total accumulated bits
inline long double allClickedBits = 0.0L; // Total accumulated bits from clicks

inline long double hotfixMult = 1.0L; // Multiplier for hotfixes (upgrades)

inline long double bitsPerSecond = 0.0L; // Current bits per second
inline long double bitsPerClick = 1.0L; // Bits per click
inline long double clickMultiplier = 1.0L; // Click multiplier

inline long double bytes = 0.0L; // Current bytes (from Re-initializing)
inline long long timesInitialized = 0LL; // Re-initializing is "Ascending"
inline long double bitsToBytesRate = 1e-8;
inline long double bitMultiplier = 1.0L;
inline long double byteMultiplier = 1.0L;
inline float patch_1Mult = 1.0f;
inline bool showConfirmPopup = false;

inline long double malbytes = 0.0L;

inline sf::Font jetBrainsMono("Assets/Font/JetBrainsMono.ttf");