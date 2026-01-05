#include "Misc/GFunctions.hpp"
#include "Misc/GIncludes.hpp"
#include "Misc/GVariables.hpp"

#include "Bits/Format.hpp"
#include "Bits/Offline.hpp"

#include "LogicGate/Hotfixes.hpp"
#include "LogicGate/LogicGate.hpp"

#include "Re-initialization/Initialization.hpp"
#include "Re-initialization/Re-initialization.hpp"
#include "Re-initialization/RootDir.hpp"

#include "UserData/Local/Loading.hpp"
#include "UserData/Local/Saving.hpp"

int main() {
	hideConsole();

	sf::RenderWindow gameWindow(sf::VideoMode({ 1920, 1080 }), "Void.INC | " + voidVersion, sf::State::Fullscreen);
	gameWindow.setFramerateLimit(60);
	gameWindow.setIcon(sf::Image("icon.png"));

	sf::Vector2f center = { 1920 / 2, 1080 / 2 };
	float wHeight = gameWindow.getSize().y;
	float wWidth = gameWindow.getSize().x;

	time_t timeEnd = 0;

	bool isLMBPressed = false;

	sf::Clock deltaClock;
	sf::Clock elapsedClock;
	sf::Clock fluctuationClock;

	sf::RectangleShape clickRect;
	clickRect.setSize(sf::Vector2f(256, 256));
	clickRect.setOrigin(sf::Vector2f(clickRect.getSize().x / 2, clickRect.getSize().y / 2));
	clickRect.setPosition(center);
	clickRect.setOutlineColor(sf::Color::Black);
	clickRect.setOutlineThickness(5);
	sf::Vector2f clickAreaSize = clickRect.getSize();

	Vortex vortex;
	vortex.vortex = sf::VertexArray(sf::PrimitiveType::LineStrip, 4000);
	float vortexScale = 1.f;

	sf::VertexArray lines(sf::PrimitiveType::Lines);
	for (int i = 0; i < wHeight; i += 4) {
		lines.append(sf::Vertex{ sf::Vector2f(0.f, (float)i), sf::Color(255, 255, 255, 40) });
		lines.append(sf::Vertex{ sf::Vector2f((float)wWidth, (float)i), sf::Color(255, 255, 255, 40) });
	}

	sf::Text bitsText(jetBrainsMono);
	bitsText.setCharacterSize(36);
	bitsText.setFillColor(sf::Color(243,238,225));

	sf::Text bitsPerSecondText(jetBrainsMono);
	bitsPerSecondText.setCharacterSize(20);
	bitsPerSecondText.setFillColor(sf::Color(243, 238, 225));

	sf::Text bytesText(jetBrainsMono);
	bytesText.setCharacterSize(36);
	bytesText.setFillColor(sf::Color(180, 0, 255));

	initLogicGates();
	initHotfixes();
	initRootTree();
	load(timeEnd, bits, bytes, allBits, allClickedBits, bitsPerSecond, hotfixMult, timesInitialized, logicGateList, hotfixList, rootTree);
	offline(timeEnd, bits, allBits, bitsPerSecond, hotfixMult);
	
	while (gameWindow.isOpen()) {
		if (rootTree[1].patched && root1Clock.getElapsedTime().asSeconds() >= 60.f) {
			root1CurrentMult = 1.1f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (1.3f - 1.1f)));
			root1Clock.restart();
		}

		bitsToBytesRate = 1e-8L;
		if (rootTree[0].patched) bitsToBytesRate = 1e-6L;
		if (rootTree[3].patched) bitsToBytesRate = 1e-7L;

		bitMultiplier = 1.0L;
		byteMultiplier = 1.0L;
		if (rootTree[2].patched) bitMultiplier *= 2.0L;
		if (rootTree[3].patched) bitMultiplier *= 4.0L;
		if (rootTree[4].patched) byteMultiplier *= (1.0L + (bytes * 0.001L));

		long double realBitsPerSecond = (bitsPerSecond * hotfixMult * bitMultiplier * root1CurrentMult) * byteMultiplier;
		float deltaTime = deltaClock.restart().asSeconds();
		float elapsedTime = elapsedClock.getElapsedTime().asSeconds();

		bits += realBitsPerSecond * deltaTime; allBits += realBitsPerSecond * deltaTime;

		updateVortex(vortex, center, elapsedTime, vortexScale);
		vortexScale += (1.f - vortexScale) * 0.1f;

		bitsText.setString("-" + formatBits(bits) + " Bits");
		centerText(bitsText, { clickRect.getPosition().x, clickRect.getPosition().y + 400 });

		bitsPerSecondText.setString("-" + formatBits(realBitsPerSecond, true) + " Bits per Second");
		centerText(bitsPerSecondText, { clickRect.getPosition().x, clickRect.getPosition().y + 440 });

		bytesText.setString("-" + formatBits(bytes, true) + " Bytes");
		centerText(bytesText, { clickRect.getPosition().x, clickRect.getPosition().y + 400 });

		sf::RenderStates states;
		states.blendMode = sf::BlendAdd;

		gameWindow.clear(sf::Color::Black);

		if (!initialization) {
			gameWindow.draw(bitsText);
			gameWindow.draw(bitsPerSecondText);

			updateLogicGateUI(gameWindow, allBits);
			drawLogicGates(gameWindow, allBits);
			drawHotfixes(gameWindow);

			drawAscensionUI(gameWindow, bits);

			gameWindow.draw(vortex.vortex, states);

			if (showConfirmPopup) {
				drawConfirmPopup(gameWindow, initialization);
			}
		}

		if (initialization) {
			if (currentInitStep == InitState::IDLE) {
				currentInitStep = InitState::VORTEX_EXPANSION;
				canClick = false;
			}
			initTimer += deltaTime;
			sf::View shakeView = gameWindow.getDefaultView();

			switch (currentInitStep) {
			case InitState::VORTEX_EXPANSION: {
				vortexScale += (20.f - vortexScale) * 0.05f;

				float intensity = (vortexScale / 20.f) * 15.f;
				float offsetX = (std::rand() % 100 - 50) / 50.f * intensity;
				float offsetY = (std::rand() % 100 - 50) / 50.f * intensity;
				shakeView.move({ offsetX, offsetY });

				gameWindow.setView(shakeView);
				gameWindow.draw(vortex.vortex, states);

				if (initTimer >= 0.5f) currentInitStep = InitState::VORTEX_SHRINK;
				break;
			}

			case InitState::VORTEX_SHRINK: {
				vortexScale += (0.f - vortexScale) * 0.15f;
				shakeView.move({ (std::rand() % 10 - 5) / 2.f, (std::rand() % 10 - 5) / 2.f });

				gameWindow.setView(shakeView);
				gameWindow.draw(vortex.vortex, states);

				if (initTimer >= 0.7f) {
					vortexScale = -10.f;
					currentInitStep = InitState::LOADING_BAR;
				}
				break;
			}

			case InitState::LOADING_BAR:
				loadingProgress = (initTimer - 0.7f) / 10.0f;

				if (loadingProgress >= 1.0f) {
					loadingProgress = 1.0f;
					currentInitStep = InitState::ROOTDIR;
				}

				drawLoadingUI(gameWindow, loadingProgress);
				break;

			case InitState::ROOTDIR:
				gameWindow.setView(gameWindow.getDefaultView());
				loadingProgress = 0.0f;
				initTimer = 0.0f;
				canClickInit = true;

				drawRootDirectory(gameWindow, bytes);
				gameWindow.draw(bytesText);
				break;
			}
		}

		gameWindow.draw(lines);

		gameWindow.display();

		while (const std::optional gameEvent = gameWindow.pollEvent()) {
			if (gameEvent->is<sf::Event::Closed>()) {
				time_t timeStart = time(nullptr);
				save(timeStart, bits, bytes, allBits, allClickedBits, bitsPerSecond, hotfixMult, timesInitialized, logicGateList, hotfixList, rootTree);
				gameWindow.close();
			}

			if (gameEvent->is<sf::Event::MouseWheelScrolled>()) {
				const auto& wheel = gameEvent->getIf<sf::Event::MouseWheelScrolled>();
				scrollOffset -= wheel->delta * 20.f;
				if (scrollOffset < 0) scrollOffset = 0;
			}

			if (gameEvent->is<sf::Event::MouseButtonPressed>()) {
				const auto& mouseEvent = gameEvent->getIf<sf::Event::MouseButtonPressed>();
				sf::Vector2i mousePos = sf::Mouse::getPosition(gameWindow);
				if (mouseEvent->button == sf::Mouse::Button::Left && canClick) {
					sf::Vector2f worldMousePos = gameWindow.mapPixelToCoords(mousePos, uiView);

					if (clickRect.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
						long double bitsClicked = bitsPerClick * clickMultiplier * (1 + realBitsPerSecond * 0.05f);
						vortexScale = 1.1f; bits += bitsClicked; allBits += bitsClicked; allClickedBits += bitsClicked;
					}

					for (auto& logGate : logicGateList) {
						if (logGate.rect.getGlobalBounds().contains(worldMousePos)) {
							if (bits >= logGate.currentBits) {
								bits -= logGate.currentBits;
								logGate.ver++;

								bitsPerSecond += logGate.bps;

								logGate.currentBits = logGate.baseBits * std::pow(logicGateInflation, logGate.ver);
							}
						}
					}

					for (auto& hf : hotfixList) {
						if (hf.written == 0 && hf.rect.getGlobalBounds().contains(worldMousePos)) {
							if (bits >= hf.bits) {
								bits -= hf.bits;
								hf.written = 1;
								hotfixMult += hf.bitMult;
							}
						}
					}
				}

				if (mouseEvent->button == sf::Mouse::Button::Left && canClickInit) {
					sf::Vector2f screenMouse = gameWindow.mapPixelToCoords(mousePos, gameWindow.getDefaultView());

					for (size_t i = 0; i < rootTree.size(); ++i) {
						auto& patch = rootTree[i];
						if (patch.patched == 0 && patch.rect.getGlobalBounds().contains(screenMouse)) {

							bool canBuy = false;
							if (i == 0) canBuy = true;
							else if (i < 3 && rootTree[i - 1].patched == 1) canBuy = true;
							else if (i >= 3 && rootTree[2].patched == 1) canBuy = true;

							if (canBuy && bytes >= patch.bytes) {
								bytes -= patch.bytes;
								patch.patched = 1;

								if (patch.name == "Patch_0") {
									bitsToBytesRate = 1e-6L;
								}
								if (patch.name == "Patch_3_1") {
									bitsToBytesRate = 1e-7L;
								}
							}
						}
					}
				}
			}
		}
	}
}