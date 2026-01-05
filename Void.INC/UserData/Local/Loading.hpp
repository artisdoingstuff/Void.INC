#pragma once

#include "../../Misc/GIncludes.hpp"

inline void load(
	time_t& timestamp,
	long double& bits,
	long double& bytes,
	long double& allBits,
	long double& allClickedBits,
	long double& bitsPerSecond,
	long double& hotfixMult,
	long long& timesInitialized,
	std::vector<LogicGate>& logicGate,
	std::vector<Hotfix>& hotfix,
	std::vector<RootPatch>& root
) {
	std::ifstream file("save.json", std::ios::in);
	if (!file.is_open()) {
		std::cerr << "No save file found." << std::endl;
		timestamp = time(nullptr);
		return;
	}

	json saveData; file >> saveData;

	timestamp = saveData["T"];
	bits = saveData["B"];
	bytes = saveData["BY"];
	allBits = saveData["AB"];
	allClickedBits = saveData["ACB"];
	bitsPerSecond = saveData["BPS"];
	hotfixMult = saveData["HFM"];
	timesInitialized = saveData["TINIT"];

	std::vector<json> savedLogicArray = saveData["LOGIC"];
	std::vector<json> savedHotfixArray = saveData["HOTFIX"];
	std::vector<json> savedRoot = saveData["ROOT"];
	hotfixMult = 1.0L;

	for (const auto& item : savedLogicArray) {
		std::string name = item["n"];
		int ver = item["v"];

		for (auto& lg : logicGate) {
			if (lg.name == name) {
				lg.ver = ver;
				lg.currentBits = lg.baseBits * std::pow(logicGateInflation, lg.ver);
				break;
			}
		}
	}

	for (const auto& item : savedHotfixArray) {
		std::string name = item["n"];
		int isWritten = item["w"];

		for (auto& hf : hotfix) {
			if (hf.name == name) {
				hf.written = isWritten;

				if (hf.written == 1) {
					hotfixMult += hf.bitMult;
				}
				break;
			}
		}
	}

	for (const auto& item : savedRoot) {
		for (auto& actualPatch : rootTree) {
			if (actualPatch.name == item["n"]) {
				actualPatch.patched = item["p"];
			}
		}
	}
}