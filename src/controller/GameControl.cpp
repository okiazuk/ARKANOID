#include "controller/GameControl.hpp"
#include <allegro5/allegro.h>

void GameControl::processInput(Racket& racket)
{
    ALLEGRO_KEYBOARD_STATE ks;
    al_get_keyboard_state(&ks);
    if (al_key_down(&ks, ALLEGRO_KEY_LEFT)) {
        racket.moveLeft();
    }
    if (al_key_down(&ks, ALLEGRO_KEY_RIGHT)) {
        racket.moveRight();
    }
}

void GameControl::update(Board& board, Ball& ball, Racket& racket, GameStats& stats)
{
    // Update ball position
    ball.update();

    // TODO: Implement collision detection with walls, paddle, and bricks

    // TODO: Update game stats and game over flag
}
