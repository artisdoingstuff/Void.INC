#pragma once

#include "../Misc/GIncludes.hpp"

inline std::string format(long double bits, bool forceDecimals = false) {
   if (bits < 1000.L) {
       if (forceDecimals) {
           std::ostringstream oss;
           oss << std::fixed << std::setprecision(2) << bits;
           return oss.str();
       }
       else return std::to_string(static_cast<long> (bits));
    }

    const std::string bitsSuffix[] = {
        "", "K", "M", "B", "T", "Qa", "Qi", "Sx", "Sp", "Oc", "No", "Dc",
        "Ud", "Dd", "Td", "Qad", "Qid", "Sxd", "Spd", "Ocd", "Nvd", "Vg",
        "Uvg", "Dvg", "Tvg", "Qavg", "Qivg", "Sxvg", "Spvg", "Ocvg", "Nvvg", "Tg",
        "Utg", "Dtg", "Ttg", "Qattg", "Qitg", "Sxtg", "Sptg", "Octg", "Nvtg", "Qg"
    };

    int tier = static_cast<int>(std::floor(std::log10(bits) / 3));

    if (tier >= std::size(bitsSuffix)) return "Infinity";

    long double val = bits / std::pow(10, tier * 3);

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << val << bitsSuffix[tier];
    return oss.str();
}