#pragma once

#include "../Misc/GIncludes.hpp"
#include "../Re-initialization/Initialization.hpp"

inline void offline(time_t timestamp, long double& bits, long double& allBits, long double bitsPerSecond, long double hotfixMult) {
	time_t elapsedTime = time(nullptr) - timestamp;
	if (elapsedTime > 0) {
		long double offlineBits = (elapsedTime * bitsPerSecond * hotfixMult) * 1;
		if (rootTree[2].patched) offlineBits *= 2.0L;
		if (rootTree[3].patched) offlineBits *= 4.0L;
		if (rootTree[4].patched) offlineBits *= (1.0L + (bytes * 0.001L));
		bits += offlineBits; allBits += offlineBits;
	}
}