/*
Project: Arkanoid 
Author: 566700
Description: breakout like game in cpp using allegro GUI
*/

#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include "controller/GameControl.hpp"
#include "model/Ball.hpp"
#include "model/Board.hpp"
#include "model/GameStats.hpp"
#include "model/Racket.hpp"
#include "view/GameScreen.hpp"
#include "utils/LevelLoader.hpp"
#include <map>






int main(int argc, char* argv[]) {


    std::cout << "[MAIN] GAME STARTING.." << std::endl; //logs in terminal

    // Initialize game objects

    Board board = Board();
    Ball ball = Ball();
    Racket racket = Racket();
    GameStats stats = GameStats();
    GameScreen view = GameScreen();
    GameControl controller = GameControl();


    // Initialize Allegro
    if (!view.init()) {
        return 1;
    }

    // MAIN LOOP
    while (controller.isRunning()) {

        controller.processInputs(racket, ball, board, stats);

        view.draw(board, ball, racket, stats);
    }

    view.destroy();
    return 0;
}

