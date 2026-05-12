#pragma once
#include "Card.hpp"
#include <vector>
#include <memory>

enum class Difficulty
{
	NoAI,
	Dumb,
	Smart,
	Unfair
};

class AI
{
private:
	Suit bestSuit;
	bool smoked = false;
	Difficulty difficulty = Difficulty::Smart;
public:
	void changeDifficulty(Difficulty difficulty);
	Difficulty getDifficulty();
	void smokeBomb();
	bool isSmoked();
	Suit getBSuit();
	void setBSuit(Suit newBestSuit);
};

