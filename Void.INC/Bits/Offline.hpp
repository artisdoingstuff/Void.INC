#pragma once

#include "../Misc/GIncludes.hpp"
#include "../Re-initialization/Initialization.hpp"

inline void offline(time_t timestamp, long double& bits, long double& allBits, long double bitsPerSecond, long double hotfixMult) {
	time_t elapsedTime = time(nullptr) - timestamp;
	if (elapsedTime > 0) {
		long double rootMult = 1.0L;
		if (rootTree[1].patched) rootMult *= 1.6L;
		if (rootTree[2].patched) rootMult *= 2.0L;
		if (rootTree[3].patched) rootMult *= 5.5L;
		if (rootTree[4].patched) {
			long double patch3_2Mult = 1.0L + (bytes * 0.002L);
			rootMult = std::min(patch3_2Mult, 100.0L);
		}
		long double offlineBits = (elapsedTime * bitsPerSecond * hotfixMult * rootMult) * 1;
		
		bits += offlineBits; allBits += offlineBits;
	}
}