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


int main(int argc, char* argv[]) {


    std::cout << "[MAIN] GAME STARTING.." << std::endl; //logs in terminal

    // Initialize LEVEL 1
     std::vector<std::vector<Brick>> level = loadLevel("../src/model/levels/level_1.txt");

    // Fallback to default if loading fails
    if (level.empty()) {
        std::cout << "Failed to load level, creating default..." << std::endl;
        level = std::vector<std::vector<Brick>>(8, std::vector<Brick>(14, Brick(1)));
    }

    // Initialize game objects

    Board board = Board(level);
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

        if(controller.isGameRunning()){
            controller.processGameInput(racket, ball);
            if (controller.isBallLaunched()) 
            {controller.update(board, ball, racket, stats);}
            view.drawGame(board, ball, racket, stats);
        }else {
            view.drawEndGame();
            controller.processMenuInput();
        }
    }

    view.destroy();
    return 0;
}

