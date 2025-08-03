#pragma once

enum class GameStates
{
	WELCOME,
	IN_GAME,
	END_GAME,
};

// These states correspond to the screen currently displayed

extern GameStates CURRENT_GAME_STATE; // used in controller and view, I found it useful making this global because game state should be accessed everywhere