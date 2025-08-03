#include "GameStats.hpp"

void GameStats::loseLife()
{
	basic_infos_.lives -= 1;
}

void GameStats::addScore(int points)
{
	basic_infos_.score += points;
}

void GameStats::gainLife()
{
	basic_infos_.lives += 1;
}

void GameStats::reset()
{
	basic_infos_.score = 0;
	basic_infos_.lives = DEFAULT_NUMBER_OF_LIVES;
}

const BasicInfo &GameStats::getBasicInfos() const
{
	return basic_infos_;
}

void GameStats::setGameOverFlag(bool flag)
{
	basic_infos_.game_over = flag;
}