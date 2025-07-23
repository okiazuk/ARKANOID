#include "controller/GameControl.hpp"

#include <allegro5/allegro.h>
#include <math.h>
#include <iostream>

void GameControl::processInput(Racket &racket, Ball &ball)
{
    ALLEGRO_KEYBOARD_STATE ks;
    al_get_keyboard_state(&ks);

    const int racket_x = racket.getPositions().x;
    const int racket_speed = racket.getParameters().speed;
    const int racket_width = racket.getParameters().width;
    const int ball_x = ball.getPositions().x;

    if (al_key_down(&ks, ALLEGRO_KEY_LEFT))
    {
        if (racket_x - racket_speed >= 0)
        {
            racket.moveLeft();
            const int new_racket_x = racket.getPositions().x;


            if (!game_starting_flag_)
            {
                ball.setPosition(new_racket_x + (racket_width) / 2);
            }

        }
    }
    if (al_key_down(&ks, ALLEGRO_KEY_RIGHT))
    {
        if (racket_x + racket_width + racket_speed <= SCREEN_WIDTH)
        {

            racket.moveRight();
            const int new_racket_x = racket.getPositions().x;
            
            if (!game_starting_flag_)
            {
                ball.setPosition(new_racket_x + (racket_width) / 2);
            }

        }
    }
    if (al_key_down(&ks, ALLEGRO_KEY_SPACE))
    {
        game_starting_flag_ = true;
    }
}




void GameControl::update(Board &board,
                         Ball &ball,
                         Racket &racket,
                         GameStats &stats)
{
    // Update ball position
    checkWallCollisions(ball);
    checkRacketCollisions(ball, racket);
    ball.update();

    if (ball.isLost()){
        handleBallLost(stats, ball, racket);
    }

    // TODO: Implement collision detection with walls, paddle, and bricks

    // TODO: Update game stats and game over flag
}

void GameControl::checkWallCollisions(Ball &ball)
{
    BallDirection direction = ball.getDirection();
    const int next_x = ball.getPositions().x + direction.x;
    const int next_y = ball.getPositions().y + direction.y;
    const int ball_radius = ball.getParameters().radius;

    // HANDLING SCREEN COLLISIONS
    if (next_x + ball_radius >= SCREEN_WIDTH || next_x - ball_radius <= 0)
    {
        ball.setDirection(-direction.x, direction.y);
    }
    else if (next_y <= UI_TEXT_HEIGHT*7)
    {
        ball.setDirection(direction.x, -direction.y);
    }
    else if (next_y >= SCREEN_HEIGHT)
    {
        std::cout << "[BALL] You lost a ball" << std::endl;
    }
}

// TODO REFACTORISATION
void GameControl::checkRacketCollisions(Ball &ball, Racket &racket)
{
    BallDirection direction = ball.getDirection();
    const int ball_x = ball.getPositions().x;
    const int ball_y = ball.getPositions().y;
    const int ball_speed = ball.getParameters().speed;
    const int ball_radius = ball.getParameters().radius;
    const int racket_x = racket.getPositions().x;
    const int racket_y = racket.getPositions().y;
    const int racket_height = racket.getParameters().height;
    const int racket_width = racket.getParameters().width;

    if ((ball_y + ball_radius >= racket_y) &&
        (ball_y + ball_radius <= racket_y + racket_height) &&
        ((ball_x + ball_radius >= racket_x) &&
             (ball_x <= racket_x + racket_width) ||
         ((ball_x >= racket_x) &&
          (ball_x - ball_radius <= racket_x + racket_width))))

    {
        int racket_hit_x = ball_x - racket_x;
        const int racket_middle = RACKET_DEFAULT_WIDTH / 2;
        const int racket_middle_range = 5; // player can more easily get the middle
        int new_dx;
        int new_dy;

        // case where ball hits middle
        if ((racket_hit_x > racket_middle - racket_middle_range) &&
            (racket_hit_x < racket_middle + racket_middle_range))
        {
            new_dy = -ball_speed;
            new_dx = 0;
        }
        else
        {
            if (racket_hit_x < racket_middle)
            {
                int angle_rad =
                    (30 + 120 * (1 - (racket_hit_x / racket_width))) * 3.14 / 180.0;
                new_dy = -sin(angle_rad) * ball_speed;
                new_dx = cos(angle_rad) * ball_speed;
            }
            else if (racket_hit_x > racket_middle)
            {
                racket_hit_x = racket_width - racket_hit_x;
                int angle_rad =
                    (30 + 120 * (1 - (racket_hit_x / racket_width))) * 3.14 / 180.0;
                new_dy = -sin(angle_rad) * ball_speed;
                new_dx = -cos(angle_rad) * ball_speed;
            }
        }
        ball.setDirection(new_dx, new_dy);
    }
}

bool GameControl::isGameRunning() const
{
    return running_flag_;
}

bool GameControl::isGameStarting() const
{
    return game_starting_flag_;
}


void GameControl::handleBallLost(GameStats& stats, Ball& ball, Racket& racket){

    const int lives = stats.getBasicInfos().lives;

    if (lives > 1)
    {stats.loseLife();
    ball.reset();
    racket.reset();
    game_starting_flag_ = false;

    }else {
        stats.setGameOverFlag(true);
        stats.reset();
        running_flag_ = false; // end of the game
    }



}