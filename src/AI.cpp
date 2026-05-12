#include "AI.hpp"
#include "Debug.hpp"
#include <string>

void AI::changeDifficulty(Difficulty newDifficulty)
{
	difficulty = newDifficulty;
}

Difficulty AI::getDifficulty()
{
	return difficulty;
}

void AI::smokeBomb()
{
	if (!smoked) smoked = true;
	else smoked = false;
}

bool AI::isSmoked()
{
	return smoked;
}

Suit AI::getBSuit()
{
	return bestSuit;
}

void AI::setBSuit(Suit newBestSuit)
{
	bestSuit = newBestSuit;
}