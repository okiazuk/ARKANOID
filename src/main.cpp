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

    // Initialize LEVEL 1
    std::map<int, std::vector<std::vector<Brick>>> level_map ;

    level_map[1] = loadLevel(LEVEL1_PATH);

    int current_level_number = 1;
    std::vector<std::vector<Brick>> current_level = level_map[current_level_number];

     // Fallback to default if loading fails
    if (current_level.empty()) {
        std::cout << "Failed to load level " << current_level_number << ", creating default..." << std::endl;
        current_level = std::vector<std::vector<Brick>>(8, std::vector<Brick>(14, Brick(0)));
    }

    // Initialize game objects

    Board board = Board(current_level, current_level_number);
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

        controller.processInputs(racket, ball);


        if (controller.isBallLaunched()) {

            controller.update(board, ball, racket, stats);
        }


        view.draw(board, ball, racket, stats);
    }

    view.destroy();
    return 0;
}

