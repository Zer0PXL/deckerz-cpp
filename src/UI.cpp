#include "UI.hpp"
#include <SFML/Graphics.hpp>
#include "Debug.hpp"
using namespace sf;
using namespace std;

sf::Texture playBtnTexture("res/play.png");

void UI::setMouse(sf::Vector2f mPos)
{
	mousePos = mPos;
}

void UI::resetInteracts()
{
	if (!interacts.playedCards.empty())
		interacts.playedCards.clear();
	interacts.playerInteraction = PI::None;
}

Interactions UI::getInteracts()
{
	return interacts;
}

void UI::resetMCs()
{
	Debug::log("[UI.cpp] ! - Resetting middleCards... (and buttons)");
	middleCards.clear();
	buttons.clear();
}

Interactions UI::inputHandler(sf::RenderWindow& window, optional<Event> event)
{
	if (event->is<sf::Event::Closed>())
		window.close();
	
	if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
	{
		if (keyPressed->scancode == sf::Keyboard::Scancode::D)
		{
			Debug::log("[UI.cpp] Draw interaction sent.");
			interacts.playerInteraction = PI::Draw;
			return interacts;
		}
	}

	if (const auto* mbPressed = event->getIf<sf::Event::MouseButtonPressed>())
	{
		if (mbPressed->button == sf::Mouse::Button::Left)
		{
			for (auto& [ucid, middle] : middleCards) 
			{
				if (middle.sprite.getGlobalBounds().contains(mousePos) && !middle.state.selected && middle.state.selectable)
				{
					middle.state.selected = true;
					selectedCards.emplace(middle.logicCard->getID(), middle);
				}
				else if (middle.sprite.getGlobalBounds().contains(mousePos))
				{
					middle.state.selected = false;
					selectedCards.erase(middle.logicCard->getID());
				}
			}

			for (auto& button : buttons)
			{
				if (!(selectedCards.empty()) && 
					button.getSprite().getGlobalBounds().contains(mousePos) && 
					button.getName() == "play")
				{
					Debug::log("[UI.cpp] Play interaction sent along with selected cards.");
					interacts.playerInteraction = PI::Play;
					interacts.playedCards = std::move(selectedCards);
					return interacts;
				}
			}
		}
	}

	return interacts;
}

void UI::layoutHandler(sf::RenderWindow& window, Snapshot snapshot)
{
	CardState state;
	CardState pileState; pileState.selectable = false; // The pile isn't selectable for obvious reasons.
	sf::Vector2f pilePos = {windowWidth/2.f, windowHeight/2.f};
	vector<shared_ptr<Card>> sorted = Hand::sortHand(snapshot.playerHand);

	// Laying out the player's hand and adding each card to the middleCards map
	for (int i = 0; i < snapshot.playerHand.size(); i++)
	{
		auto emplacedSuccesfully = middleCards.try_emplace(snapshot.playerHand[i]->getID(), GFX::layoutCard(snapshot.playerHand[i], {GFX::calculateHandPos(snapshot.playerHand)[i], 600.f}, state));
		auto existingState = emplacedSuccesfully.first->second.state;
		if (!emplacedSuccesfully.second)
			middleCards.insert_or_assign(snapshot.playerHand[i]->getID(), GFX::layoutCard(snapshot.playerHand[i], {GFX::calculateHandPos(snapshot.playerHand)[i], 600.f}, existingState));
	}

	// Laying out the pile card and adding it to the middleCards map
	middleCards.insert_or_assign(snapshot.pileCard->getID(), GFX::layoutCard(snapshot.pileCard, pilePos, pileState));

	Button button("play", playBtnTexture, GameState::Play);
	buttons.push_back(button);
}

void UI::collisionHandler()
{
	// Collision detection
	for (auto& [ucid, middle] : middleCards) 
	{
		if (middle.sprite.getGlobalBounds().contains(mousePos)) middle.state.hovered = true;
		else middle.state.hovered = false;
	}
}

void UI::drawHandler(sf::RenderWindow& window)
{
	// Iterate over every middleCard and draw it
	for (auto& [ucid, mC] : middleCards)
	{
		GFX::drawCard(mC, window);
	}

	// Iterate over every button and draw it
	for (auto& button : buttons)
	{
		window.draw(button.getSprite());
	}
}