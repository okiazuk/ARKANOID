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


int main(int argc, char* argv[]) {


    std::cout << "[MAIN] GAME STARTING.." << std::endl; //logs in terminal

    // Initialize Allegro and game objects
    std::vector<std::vector<Brick>> test;
    Board board = Board(test);
    Ball ball = Ball();
    Racket racket = Racket();
    GameStats stats = GameStats();
    GameScreen view = GameScreen();
    GameControl controller = GameControl();


    if (!view.init()) {
        return 1;
    }

    while (controller.isGameRunning()) {
        controller.processInput(racket, ball);
        if (controller.isGameStarting()) 
        {controller.update(board, ball, racket, stats);}
        view.draw(board, ball, racket, stats);
    }

    view.destroy();
    return 0;
}

