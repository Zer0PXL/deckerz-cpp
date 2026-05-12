#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include "Card.hpp"
#include "Hand.hpp"
#include "Deck.hpp"
#include "Table.hpp"
#include "PlayState.hpp"
#include "Debug.hpp"
#include "AI.hpp"
#include "Consumable.hpp"
#include "Inventory.hpp"
#include <memory>
#include "GFX.hpp"
#include "Button.hpp"
#include "UI.hpp"
using namespace std;
using namespace sf;

#ifdef DEBUG
bool debugMode = true;
bool cheats = true;
#else
bool debugMode = false;
bool cheats = false;
#endif

int main()
{
	std::cout << "Deckerz GUI v0\n";

	// SFML graphics objects
	RenderWindow window(VideoMode({windowWidth, windowHeight}), "Deckerz GUI alpha 0");

	sf::Texture bgTexture("res/bg.png");
	sf::Sprite bpsprite(bgTexture);

	sf::Vector2f mousePos;

	// My graphics objects
	UI ui;

	// State Manager
	StateManager stateMan;
	stateMan.setState(GameState::Play); // temporary

	// PlayState
	PlayState ps;
	ps.resetBonuses();

	// Start of SFML window loop
	while (window.isOpen())
	{
		// Initialize stuff
		auto mousePos = window.mapPixelToCoords(Mouse::getPosition(window));
		ui.resetInteracts();
		ui.setMouse(mousePos);

		// SFML events
		while (const optional event = window.pollEvent()) ui.inputHandler(window, event);
		
		// If it's the AI's turn, the AI plays
		if (ps.getTurn() == Turn::AI) ps.aiTurn();
		// Else, the player gets to play
		else ps.PIHandler(ui.getInteracts());

		// Clear window and draw background
		window.clear();
		window.draw(bpsprite);

		// Layout everything
		if (ui.getInteracts().playerInteraction == PI::Play) ui.resetMCs();
		Snapshot snapshot = ps.snapshot();
		ui.layoutHandler(window, snapshot);

		// Check collision
		ui.collisionHandler();

		// Draw everything
		ui.drawHandler(window);

		// Display
		window.display();
	}

	return 0;
}