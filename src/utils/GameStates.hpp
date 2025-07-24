#pragma once

enum class GameStates {
	MAIN_MENU,
	IN_GAME,
	END_GAME,
};

extern GameStates CURRENT_GAME_STATE;