#include "controller/GameControl.hpp"
#include <string>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>

void GameControl::processInputs(Racket &racket, Ball &ball, Board& board, GameStats& stats)
{

    if (CURRENT_GAME_STATE == GameStates::MAIN_MENU)
    {
        processMenuInput();
    }
    else if (CURRENT_GAME_STATE == GameStates::IN_GAME)
    {

        if (isBallLaunched()) {
            update(board, ball, racket, stats);
        }

        processGameInput(racket, ball);

    } else if (CURRENT_GAME_STATE == GameStates::END_GAME){

        processEndGameInput();
    }
}

void GameControl::processGameInput(Racket &racket, Ball &ball)
{
    al_get_keyboard_state(&ks_);
    const float racket_x = racket.getPositions().x;
    const int racket_speed = racket.getParameters().speed;
    const int racket_width = racket.getParameters().width;

    // LATER FOR INPUT OPTION BETWEEN MOUSE AND KEYBOARD
    ALLEGRO_MOUSE_STATE ms;
    al_get_mouse_state(&ms);
    // Mouse control
    int new_racket_x = ms.x - racket_width / 2;

    if (new_racket_x < 0)
    {
        new_racket_x = 0;
    }
    if (new_racket_x + racket_width > SCREEN_WIDTH)
    {
        new_racket_x = SCREEN_WIDTH - racket_width;
    }
    racket.setPosition(new_racket_x);

    // Keyboard control (overrides mouse if keys are pressed)
    if (al_key_down(&ks_, ALLEGRO_KEY_LEFT))
    {
        if (racket_x - racket_speed >= 0)
        {
            racket.moveLeft();
        }
    }
    if (al_key_down(&ks_, ALLEGRO_KEY_RIGHT))
    {
        if (racket_x + racket_width + racket_speed <= SCREEN_WIDTH)
        {
            racket.moveRight();
        }
    }

    // Update ball position before game starts
    if (!ball_launched_)
    {
        ball.setPosition(racket.getPositions().x + (racket_width / 2));
    }

    // Launch ball with space or mouse click
    if (al_key_down(&ks_, ALLEGRO_KEY_SPACE))
    {
        ball_launched_ = true;
    }
}

void GameControl::processMenuInput()
{

    al_get_keyboard_state(&ks_);
    for (int i = 1; i < ALLEGRO_KEY_MAX; i++)
    {
        if (al_key_down(&ks_, i))
        {
            CURRENT_GAME_STATE = GameStates::IN_GAME;

            break;
        }
    }
    /*

    std::vector<std::vector<Brick>> level = loadLevel(LEVEL1_PATH);

    int current_level_number = 1;
    std::vector<std::vector<Brick>> current_level = level_map[current_level_number];

     // Fallback to default if loading fails
    if (current_level.empty()) {
        std::cout << " [MAIN] Failed to load level " << current_level_number << ", creating default..." << std::endl;
        current_level = std::vector<std::vector<Brick>>(8, std::vector<Brick>(14, Brick(0)));
    }
    */


}

void GameControl::processEndGameInput()
{

    al_get_keyboard_state(&ks_);
    for (int i = 1; i < ALLEGRO_KEY_MAX; i++)
    {
        if (al_key_down(&ks_, i))
        {
            CURRENT_GAME_STATE = GameStates::IN_GAME;

            break;
        }
    }
}


void GameControl::update(Board &board,
                         Ball &ball,
                         Racket &racket,
                         GameStats &stats)
{
    // Update ball position
    ball.update();
    checkBrickCollisions(ball, board, stats, racket);
    checkWallCollisions(ball);
    checkRacketCollisions(ball, racket);

    if (ball_launched_ && ball.isLost())
    {
        handleBallLost(stats, ball, racket, board);
    }

    if (hasWon(board))
    {
        stats.setGameOverFlag(false);
        saveBestScore(stats, board);
        resetGame(stats, ball, racket, board);
        CURRENT_GAME_STATE = GameStates::END_GAME;
        std::vector<std::vector<Brick>> new_level = loadLevel(LEVEL_PATH_MAP[(board.getLevelNumber()+1)%TOTAL_LEVELS]);
        int new_level_number = (board.getLevelNumber() + 1)%TOTAL_LEVELS;
        board.changeLevel(new_level, new_level_number);
    }
}

void GameControl::checkWallCollisions(Ball &ball)
{
    const BallDirection &direction = ball.getDirection();
    const float ball_x = ball.getPositions().x;
    const float ball_y = ball.getPositions().y;
    const float ball_radius = ball.getParameters().radius;
    const float ball_speed = ball.getParameters().speed;

    // HANDLING SCREEN COLLISIONS
    if (ball_x + ball_radius + ball_speed >= SCREEN_WIDTH || ball_x - ball_radius - ball_speed <= 0)
    {
        ball.setDirection(-direction.x, direction.y);
    }
    else if (ball_y - (ball_radius + ball_radius / 2) <= SEPARATION_LINE_HEIGHT)
    {
        ball.setDirection(direction.x, -direction.y);
    }
    else if (ball_y >= SCREEN_HEIGHT)
    {
        std::cout << "[BALL] You lost a ball" << std::endl;
    }
}

// TODO REFACTORISATION
void GameControl::checkRacketCollisions(Ball &ball, Racket &racket)
{
    const float ball_x = ball.getPositions().x;
    const float ball_y = ball.getPositions().y;
    const float ball_speed = ball.getParameters().speed;
    const float ball_radius = ball.getParameters().radius;
    const float racket_x = racket.getPositions().x;
    const float racket_y = racket.getPositions().y;
    const int racket_height = racket.getParameters().height;
    const int racket_width = racket.getParameters().width;


    if ((ball_y + ball_radius >= racket_y) &&
        (ball_y + ball_radius <= racket_y + racket_height) &&
        ((ball_x + ball_radius >= racket_x) && // check hit from left side
             (ball_x <= racket_x + racket_width) ||
         ((ball_x >= racket_x) && // check hit from right side
          (ball_x - ball_radius <= racket_x + racket_width))))

    {
        float racket_hit_x = ball_x - racket_x;
        const int racket_middle = RACKET_DEFAULT_WIDTH / 2;
        float new_dx;
        float new_dy;

        // case where ball hits middle
        if (racket_hit_x == racket_middle)
        {
            new_dy = -ball_speed;
            new_dx = 0;
        }
        else
        {

            if (racket_hit_x < racket_middle)
            {
                float angle_rad =
                    (30 + 120 * (1 - (racket_hit_x / racket_width))) * M_PI / 180.0;
                new_dy = -sin(angle_rad) * ball_speed;
                new_dx = cos(angle_rad) * ball_speed;
            }
            else if (racket_hit_x > racket_middle)
            {
                racket_hit_x = racket_width - racket_hit_x;
                float angle_rad =
                    (30 + 120 * (1 - (racket_hit_x / racket_width))) * M_PI / 180.0;
                new_dy = -sin(angle_rad) * ball_speed;
                new_dx = -cos(angle_rad) * ball_speed;
            }
        }
        ball.setDirection(new_dx, new_dy);
    }
}



bool GameControl::powerUpHitRacket(PowerUps& power_up, Racket& racket){

    const float power_up_x = power_up.getPositions().x;
    const float power_up_y = power_up.getPositions().y;
    const float power_up_radius = power_up.getParameters().radius;
    const float racket_x = racket.getPositions().x;
    const float racket_y = racket.getPositions().y;
    const int racket_height = racket.getParameters().height;
    const int racket_width = racket.getParameters().width;


    if ((power_up_y + power_up_radius >= racket_y) &&
        (power_up_y + power_up_radius <= racket_y + racket_height) &&
        ((power_up_x + power_up_radius >= racket_x) && // check hit from left side
             (power_up_x <= racket_x + racket_width) ||
         ((power_up_x >= racket_x) && // check hit from right side
          (power_up_x - power_up_radius <= racket_x + racket_width))))

    {
        return true;

    }
    return false;



}

void GameControl::checkBrickCollisions(Ball &ball, Board &board, GameStats &stats, Racket& racket)
{
    const float ball_x = ball.getPositions().x;
    const float ball_y = ball.getPositions().y;
    const float ball_radius = ball.getParameters().radius;
    const BallDirection &direction = ball.getDirection();
    
    // Calculate next position based on current direction
    float next_x = ball_x + direction.x;
    float next_y = ball_y + direction.y;
    
    const int board_height = board.getParameters().height;
    const int board_width = board.getParameters().width;
    auto &bricks = board.getBricks();
    
    bool collision_happened = false;
    bool horizontal_collision = false;
    bool vertical_collision = false;
    
    // Find the closest brick that the ball collides with
    for (int r = 0; r < board_height; r++) {
        for (int c = 0; c < board_width; c++) {
            Brick &brick = bricks[r][c];
            
            if (brick.isDestroyed() && !collision_happened) {
                                 
                PowerUps& power_up = brick.getPowerUp();

                if (power_up.getType() != PowerType::NONE){

                    power_up.update();
                    if(powerUpHitRacket(power_up, racket)){
                        if (power_up.getType() == PowerType::PLAYER){
                            stats.gainLife();
                        }else if (power_up.getType() == PowerType::CATCH_BALL) {

                        }else if (power_up.getType() == PowerType::INTERRUPTION){

                        }else if (power_up.getType() == PowerType::LASER){

                        }else if (power_up.getType() == PowerType::RACKET_GROW){

                        }else if (power_up.getType() == PowerType::SLOW_DOWN){
                            
                        }
                        power_up.disappear();

                    }
                }
                continue; // we don't want collision with destroyed bricks
            }
            
            const int brick_width = brick.getBrickType().width;
            const int brick_height = brick.getBrickType().height;
            
            // Brick boundaries
            const float brick_left = c * brick_width;
            const float brick_right = brick_left + brick_width;
            const float brick_top = r * brick_height + SEPARATION_LINE_HEIGHT;
            const float brick_bottom = brick_top + brick_height;
            
            // Check if ball overlaps with brick
            if (next_x + ball_radius >= brick_left && next_x - ball_radius <= brick_right &&
                next_y + ball_radius >= brick_top && next_y - ball_radius <= brick_bottom) {
                
                // Vertical collision (top/bottom)
                if (ball_x + ball_radius >= brick_left && ball_x - ball_radius <= brick_right) {
                    if ((direction.y > 0 && ball_y + ball_radius <= brick_top) ||
                        (direction.y < 0 && ball_y - ball_radius >= brick_bottom)) {
                        vertical_collision = true;
                    }
                }
                
                // Horizontal collision (left/right)
                if (ball_y + ball_radius >= brick_top && ball_y - ball_radius <= brick_bottom) {
                    if ((direction.x > 0 && ball_x + ball_radius <= brick_left) ||
                        (direction.x < 0 && ball_x - ball_radius >= brick_right)) {
                        horizontal_collision = true;
                    }
                }
                
                // Corner collision
                if (!vertical_collision && !horizontal_collision) {
                    // Check which corner is closest to the ball
                    const float closest_x = std::max(brick_left, std::min(ball_x, brick_right));
                    const float closest_y = std::max(brick_top, std::min(ball_y, brick_bottom));
                    
                    const float distance_x = ball_x - closest_x;
                    const float distance_y = ball_y - closest_y;
                    
                    // If we have a corner hit
                    if (distance_x * distance_x + distance_y * distance_y <= ball_radius * ball_radius) {
                        horizontal_collision = true;
                        vertical_collision = true;
                    }
                }
                
                // Process collision if detected
                if (horizontal_collision || vertical_collision) {
                    const int brick_points = brick.getBrickType().gained_points;
                    const float brick_middle_x = brick_left + ((brick_right - brick_left) / 2);
                    const float brick_middle_y = brick_top + ((brick_bottom - brick_top) / 2);
                    brick.hit(brick_middle_x, brick_middle_y);
                    stats.addScore(brick_points);


                    
                    // Update ball direction
                    const float new_dx = horizontal_collision ? -direction.x : direction.x;
                    const float new_dy = vertical_collision ? -direction.y : direction.y;
                    ball.setDirection(new_dx, new_dy);
                    
                    collision_happened = true;
                    break;
                }
            }
        }
    }
}


bool GameControl::hasWon(Board &board)
{

    const int board_height = board.getParameters().height;
    const int board_width = board.getParameters().width;
    auto &bricks = board.getBricks();

    for (int r = 0; r < board_height; r++)
    {
        for (int c = 0; c < board_width; c++)
        {

            Brick &brick = bricks[r][c];
            if (!brick.isDestroyed() && brick.getBrickType().color != BrickColor::GOLD)
            {
                return false;
            }
        }
    } 
    return true;
}

bool GameControl::isRunning() const
{
    return running_flag_;
}

bool GameControl::isBallLaunched() const
{
    return ball_launched_;
}

void GameControl::handleBallLost(GameStats &stats, Ball &ball, Racket &racket, Board &board)
{

    const int lives = stats.getBasicInfos().lives;

    if (lives > 1)
    {
        const int ball_speed = ball.getParameters().speed;
        stats.loseLife();
        ball.setDirection(0, -ball_speed);
    }
    else
    {
        saveBestScore(stats, board);
        resetGame(stats, ball, racket, board);
        stats.setGameOverFlag(true);
        CURRENT_GAME_STATE = GameStates::END_GAME;
    }
    ball_launched_ = false;
}

void GameControl::resetGame(GameStats &stats, Ball &ball, Racket &racket, Board &board)
{
    stats.reset();
    racket.reset();
    ball.reset();
    board.reset();
    ball_launched_ = false;
}

void GameControl::saveBestScore(GameStats &stats, Board& board)
{

    const int level_number = board.getLevelNumber();
    const int best_score = loadScore(BEST_SCORE_PATH_MAP.at(level_number));
    const int current_score = stats.getBasicInfos().score;

    std::cout << current_score << best_score << std::endl;
    if (best_score < current_score)
    {

        saveScore(current_score, BEST_SCORE_PATH_MAP.at(level_number));
    }
}