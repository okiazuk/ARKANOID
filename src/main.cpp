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
#include "model/Balls.hpp"
#include "model/Lasers.hpp"
#include "view/GameScreen.hpp"
#include "utils/LevelLoader.hpp"
#include <map>




int main(int argc, char* argv[]) {


    std::cout << "[MAIN] GAME STARTING.." << std::endl; //logs in terminal

    // Initialize game objects

    Board board = Board();
    Balls balls = Balls();
    Racket racket = Racket();
    Lasers lasers = Lasers();
    GameStats stats = GameStats();
    GameScreen view = GameScreen();
    GameControl controller = GameControl();


    // Initialize Allegro
    if (!view.init()) {
        return 1;
    }

    // MAIN LOOP
    while (controller.isRunning()) {

        controller.processInputs(racket, balls, board, stats, lasers);

        view.draw(board, balls, racket, stats, lasers);
    }

    view.destroy();


    std::cout << "[MAIN] EXITING GAME" << std::endl;
    return 0;
}

