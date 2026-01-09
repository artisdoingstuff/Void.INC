#pragma once

#include "../Misc/GIncludes.hpp"

inline void drawFolderHeader(sf::RenderWindow& window, std::string path, sf::Vector2f pos, bool isOpen) {
    sf::RectangleShape box({ 350.f, 30.f });
    box.setPosition(pos);
    box.setFillColor(sf::Color(10, 10, 10));
    box.setOutlineColor(sf::Color::White);
    box.setOutlineThickness(1.f);

    sf::Text txt(jetBrainsMono, (isOpen ? "[-] " : "[+] ") + path + (isOpen ? getCursor() : ""), 14);
    txt.setPosition(sf::Vector2f(pos.x + 8.f, pos.y + 4.f));

    window.draw(box);
    window.draw(txt);
}

inline void drawTerminalUI(sf::RenderWindow& window, long double bits, long double allBits) {
    float startX = 40.f;
    float currentY = 100.f - scrollOffset;

    bool isLogicOpen = (currentDir == Directory::LOGIC_GATES);
    drawFolderHeader(window, "root/LOGIC_GATES/", { startX, currentY }, isLogicOpen);
    currentY += 40.f;

    if (isLogicOpen) {
        for (auto& lg : logicGateList) {
            if (lg.ver > 0 || allBits >= lg.baseBits) {
                lg.rect.setPosition({ startX + 20.f, currentY });
                lg.rect.setSize({ 350.f, 45.f });

                sf::Text t(jetBrainsMono, "|-- " + lg.name + "_" + std::to_string(lg.ver) + ".bin", 12);
                t.setPosition(sf::Vector2f(startX + 25.f, currentY));
                window.draw(t);

                sf::Text sub(jetBrainsMono, "|  |-- " + lg.desc + "\n|  `-- -" + format(lg.currentBits) + "_bits.tmp", 11);
                sub.setFillColor(sf::Color(140, 140, 140));
                sub.setPosition(sf::Vector2f(startX + 25.f, currentY + 16.f));
                window.draw(sub);

                currentY += 50.f;
            }
        }
    }

    currentY += 10.f;
    bool isHotfixOpen = (currentDir == Directory::HOTFIXES);
    drawFolderHeader(window, "root/HOTFIXES/", { startX, currentY }, isHotfixOpen);
    currentY += 40.f;

    if (isHotfixOpen) {
        for (auto& hf : hotfixList) {
            std::string ext = (hf.written == 1) ? ".sys" : ".pkg";
            hf.rect.setPosition({ startX + 20.f, currentY });
            hf.rect.setSize({ 350.f, 35.f });

            sf::Text t(jetBrainsMono, "|-- " + hf.name + ext, 12);
            t.setPosition(sf::Vector2f(startX + 25.f, currentY));
            window.draw(t);

            if (hf.written == 0) {
                sf::Text cost(jetBrainsMono, "|  `-- -" + format(hf.bits) + "_bits.tmp", 11);
                cost.setFillColor(sf::Color(140, 140, 140));
                cost.setPosition(sf::Vector2f(startX + 25.f, currentY + 16.f));
                window.draw(cost);
                currentY += 40.f;
            }
            else {
                currentY += 20.f;
            }
        }
    }

    if (allBits >= 5000000.0L) {
        currentY += 10.f;
        bool isReinitOpen = (currentDir == Directory::REINIT);
        drawFolderHeader(window, "root/REINIT/", { startX, currentY }, isReinitOpen);

        if (isReinitOpen) {
            sf::Text t(jetBrainsMono, "|-- reinit.bat\n`-- reinit.stat\n   `-- -" + format(getPendingBytes(bits), true) + "_bytes_yield.log", 12);
            t.setPosition(sf::Vector2f(startX + 25.f, currentY + 40.f));
            window.draw(t);
        }
    }

    window.setView(window.getDefaultView());
}

inline float getLogicHeight() {
    float h = 0;
    for (auto& lg : logicGateList) {
        if (lg.ver > 0 || allBits >= lg.baseBits) h += 50.f; 
    }
    return h;
}

inline float getHotfixHeight() {
    float h = 0;
    for (auto& hf : hotfixList) {
        h += (hf.written == 0) ? 40.f : 20.f;
    }
    return h;
}