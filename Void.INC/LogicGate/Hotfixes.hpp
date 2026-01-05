#pragma once

#include "../Misc/GIncludes.hpp"

struct Hotfix {
    std::string name;
    long double bits;
    float bitMult;
    int written = 0;
    sf::RectangleShape rect;

    Hotfix() : bits(0), bitMult(0), written(0) {}

    Hotfix(std::string n, long double c, float m)
        : name(n), bits(c), bitMult(m), written(0) {
        rect.setSize({ 120.f, 80.f });
        rect.setOutlineColor(sf::Color(243, 238, 225));
        rect.setOutlineThickness(2.f);
    }
};

inline std::vector<Hotfix> hotfixList;
inline const float hfWidth = 120.0f;
inline const float hfHeight = 80.0f;
inline const float hfSpacing = 15.0f;

inline void initHotfixes() {
    hotfixList.push_back({ "init_patch", 100.0L, 0.01f });
    hotfixList.push_back({ "boot_optim", 250.0L, 0.01f });
    hotfixList.push_back({ "cache_clear", 600.0L, 0.01f });
    hotfixList.push_back({ "core_refactor", 1500.0L, 0.01f });
    hotfixList.push_back({ "thread_sync", 4000.0L, 0.01f });
    hotfixList.push_back({ "heap_trim", 10000.0L, 0.01f });
    hotfixList.push_back({ "stack_flush", 25000.0L, 0.01f });
    hotfixList.push_back({ "bit_stable_fix", 60000.0L, 0.01f });
    hotfixList.push_back({ "io_throttle", 150000.0L, 0.01f });
    hotfixList.push_back({ "cycle_reduce", 400000.0L, 0.01f });
    hotfixList.push_back({ "bit_flip_v1", 1000000.0L, 0.02f });
    hotfixList.push_back({ "overflow_ctrl", 2200000.0L, 0.02f });
    hotfixList.push_back({ "registry_tweak", 5000000.0L, 0.02f });
    hotfixList.push_back({ "nullptr_fix", 12000000.0L, 0.02f });
    hotfixList.push_back({ "parity_fix", 25000000.0L, 0.02f });
    hotfixList.push_back({ "mem_compress", 55000000.0L, 0.02f });
    hotfixList.push_back({ "cycle_skip", 110000000.0L, 0.02f });
    hotfixList.push_back({ "runtime_opt", 200000000.0L, 0.02f });
    hotfixList.push_back({ "clock_spd_fix", 350000000.0L, 0.02f });
    hotfixList.push_back({ "bus_overdrive", 550000000.0L, 0.02f });
    hotfixList.push_back({ "ptr_deref_opt", 800000000.0L, 0.02f });
    hotfixList.push_back({ "seg_fault_supp", 1100000000.0L, 0.02f });
    hotfixList.push_back({ "bin_compact", 1500000000.0L, 0.02f });
    hotfixList.push_back({ "gate_bypass_v1", 2000000000.0L, 0.02f });
    hotfixList.push_back({ "fetch_accel", 2600000000.0L, 0.02f });
    hotfixList.push_back({ "patch_x86_", 3300000000.0L, 0.02f });
    hotfixList.push_back({ "mem_map_align", 4100000000.0L, 0.02f });
    hotfixList.push_back({ "syscall_bypass", 5000000000.0L, 0.02f });
    hotfixList.push_back({ "data_leak_fix", 6000000000.0L, 0.02f });
    hotfixList.push_back({ "inst_set_ext", 7200000000.0L, 0.02f });
    hotfixList.push_back({ "handler_fix", 8500000000.0L, 0.03f });
    hotfixList.push_back({ "raw_input_opt", 9200000000.0L, 0.03f });
    hotfixList.push_back({ "paged_pool_ext", 9600000000.0L, 0.03f });
    hotfixList.push_back({ "driver_roll_v1", 9900000000.0L, 0.03f });
    hotfixList.push_back({ "root_grant", 10000000000.0L, 0.03f });
}

inline void drawHotfixes(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    float startX = 50.0f;
    float startY = 150.0f;

    int displayedCount = 0;
    for (auto& hf : hotfixList) {
        if (hf.written == 1) continue;

        int col = displayedCount % 3;
        int row = displayedCount / 3;

        float x = startX + (col * (hfWidth + hfSpacing));
        float y = startY + (row * (hfHeight + hfSpacing));

        hf.rect.setSize({ hfWidth, hfHeight });
        hf.rect.setPosition({ x, y });
        hf.rect.setFillColor(sf::Color::Black);

        if (hf.rect.getGlobalBounds().contains(window.mapPixelToCoords(mousePos))) {
            hf.rect.setFillColor(sf::Color(10, 10, 10));
        }

        window.draw(hf.rect);

        sf::Text text(jetBrainsMono);
        int percent = static_cast<int>(hf.bitMult * 100);
        text.setString(hf.name + "\nBits: -" + formatBits(hf.bits) + "\n+" + std::to_string(percent) + "%");
        text.setCharacterSize(14);
        text.setPosition(sf::Vector2f(x + 5.f, y + 5.f));
        window.draw(text);

        displayedCount++;
    }
}

inline void to_json(json& j, const Hotfix& h) {
    j = json{ {"n", h.name}, {"w", h.written} };
}

inline void from_json(const json& j, Hotfix& h) {
    j.at("n").get_to(h.name);
    j.at("w").get_to(h.written);
}