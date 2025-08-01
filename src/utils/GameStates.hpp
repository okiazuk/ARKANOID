#pragma once

enum class GameStates {
	WELCOME,
	IN_GAME,
	END_GAME,
};

extern GameStates CURRENT_GAME_STATE;