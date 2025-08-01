#include "controller/GameControl.hpp"
#include <string>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>

void GameControl::processInputs(Racket &racket, Balls &balls, Board &board, GameStats &stats, Lasers& lasers)
{

    if (CURRENT_GAME_STATE == GameStates::WELCOME)
    {
        processMenuInput();
    }
    else if (CURRENT_GAME_STATE == GameStates::IN_GAME)

    {

        if(!game_has_started_){
            balls.createBall();
            Ball* ball = balls.getBalls()[0];
            ball->setPosition(racket.getPositions().x + (racket.getParameters().width / 2));
            ball->setDirection(BALL_DEFAULT_DX, BALL_DEFAULT_DY);
            game_has_started_ = true;
        }
            update(board, racket, stats, balls, lasers);
            processGameInput(stats, racket, balls, lasers, board);
    }
    else if (CURRENT_GAME_STATE == GameStates::END_GAME)
    {

        processEndGameInput();
    }
}

void GameControl::processGameInput(GameStats& stats, Racket &racket, Balls &balls, Lasers& lasers, Board& board)
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

    if (!release_ball_)
    {
        for (auto& ball: balls.getBalls()) {
            const float ball_x = ball->getPositions().x;
            const float delta = ball_x - (racket_x + (racket_width / 2));
            ball->setPosition(racket.getPositions().x + (racket_width / 2) + delta);
        }
    }

    // Launch ball with space or mouse click
    if (al_key_down(&ks_, ALLEGRO_KEY_SPACE))
    
    {
        if (!space_pressed_)
        {
            release_ball_ = true;
            if (laser_on_)
            {
                lasers.createLaser(racket.getPositions().x + racket_width/2);
                laser_counter_ -= 1;
                if (laser_counter_ == 0)
                {
                    laser_counter_ = DEFAULT_NUMBER_OF_LASER;
                    laser_on_ = false;
                }
            }
        }
        space_pressed_ = true;
    }
    else // prevents multiple space bar action
    {
        space_pressed_ = false;
    }

    for (int i = ALLEGRO_KEY_0; i <= ALLEGRO_KEY_3; ++i) {
        if (al_key_down(&ks_, i)) {
            int level_num = i - ALLEGRO_KEY_0;
            resetGame(stats, balls, racket, board, lasers);
            std::vector<std::vector<Brick>> new_level = loadLevel(LEVEL_PATH_MAP[level_num]);
            board.changeLevel(new_level, level_num);
            std::cout << "[GAME CONTROL] changed to level " << level_num << std::endl;
        }
    }

    if (al_key_down(&ks_, ALLEGRO_KEY_R)){
        int score_level = board.getLevelNumber();
        saveScore(0, BEST_SCORE_PATH_MAP[score_level]);
    }
    if (al_key_down(&ks_, ALLEGRO_KEY_L)){
        running_flag_ = false;
        resetGame(stats, balls, racket, board, lasers);
        
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

void GameControl::update(Board &board, Racket &racket, GameStats &stats, Balls& balls, Lasers& lasers)
{

    // Update balls that are not lost
    for (auto* ball : balls.getBalls()) {
        if (!ball->isLost()) {
            ball->update();
            checkWallCollisions(*ball);
            checkRacketCollisions(*ball, racket);
        } else {
            handleBallLost(stats, racket, board, balls, *ball, lasers);
        }
    }
    

    for (auto* laser: lasers.getLasers()){
        if(!laser->isLost()){
            laser->update();
        }else {
            lasers.removeLaser(*laser);
        }
    }
    checkBrickCollisions(balls, board, stats, racket, lasers);


    if (hasWon(board))
    {
        stats.setGameOverFlag(false);
        saveBestScore(stats, board);
        resetGame(stats, balls, racket, board, lasers);
        CURRENT_GAME_STATE = GameStates::END_GAME;
        std::vector<std::vector<Brick>> new_level = loadLevel(LEVEL_PATH_MAP[(board.getLevelNumber() + 1) % TOTAL_LEVELS]);
        int new_level_number = (board.getLevelNumber() + 1) % TOTAL_LEVELS;
        board.changeLevel(new_level, new_level_number);
    }
}

void GameControl::checkWallCollisions(Ball &ball)
{
    const BallDirection &direction = ball.getDirection();
    const BallPositions &ball_pos = ball.getPositions();
    const float ball_radius = ball.getParameters().radius;
    const float ball_speed = ball.getParameters().speed;

    // HANDLING SCREEN COLLISIONS
    if (ball_pos.x + ball_radius + ball_speed >= SCREEN_WIDTH || ball_pos.x - ball_radius - ball_speed <= 0)
    {
        ball.setDirection(-direction.x, direction.y);
    }
    else if (ball_pos.y - (ball_radius + ball_radius / 2) <= SEPARATION_LINE_HEIGHT)
    {
        ball.setDirection(direction.x, -direction.y);
    }
    else if (ball_pos.y >= SCREEN_HEIGHT)
    {
        std::cout << "[BALL] You lost a ball" << std::endl;
    }
}

// TODO REFACTORISATION
void GameControl::checkRacketCollisions(Ball &ball, Racket &racket)
{
    const BallPositions ball_pos = ball.getPositions();
    float ball_speed = ball.getParameters().speed;
    const float ball_radius = ball.getParameters().radius;
    const RacketPositions &racket_pos = racket.getPositions();
    const RacketParameters &racket_param = racket.getParameters();

    if ((ball_pos.y + ball_radius >= racket_pos.y) &&
        (ball_pos.y + ball_radius <= racket_pos.y + racket_param.height) &&
        ((ball_pos.x + ball_radius >= racket_pos.x) && // check hit from left side
             (ball_pos.x <= racket_pos.x + racket_param.width) ||
         ((ball_pos.x >= racket_pos.x) && // check hit from right side
          (ball_pos.x - ball_radius <= racket_pos.x + racket_param.width))))

    {
        float racket_hit_x = ball_pos.x - racket_pos.x;
        const int racket_middle = RACKET_DEFAULT_WIDTH / 2;
        float new_dx;
        float new_dy;

        // case where ball hits middle
        if (racket_hit_x == racket_middle)
        {
            new_dy = BALL_DEFAULT_DY;
            new_dx = BALL_DEFAULT_DX;
        }
        else
        {

            if (racket_hit_x < racket_middle)
            {
                float angle_rad =
                    (30 + 120 * (1 - (racket_hit_x / racket_param.width))) * M_PI / 180.0;
                new_dy = -sin(angle_rad);
                new_dx = cos(angle_rad);
            }
            else if (racket_hit_x > racket_middle)
            {
                racket_hit_x = racket_param.width - racket_hit_x;
                float angle_rad =
                    (30 + 120 * (1 - (racket_hit_x / racket_param.width))) * M_PI / 180.0;
                new_dy = -sin(angle_rad);
                new_dx = -cos(angle_rad);
            }
        }
        ball.setDirection(new_dx, new_dy);

        if (ball_speed < BALL_DEFAULT_SPEED)
        { // allow the ball to restore its normal speed
            ball_speed += 0.5f;
            ball.setBallSpeed(ball_speed);

            if (ball_speed > BALL_DEFAULT_SPEED)
            {
                ball.setBallSpeed(BALL_DEFAULT_SPEED);
            }

            std::cout << "[GAME CONTROL] ball is increasing speed: " << ball_speed << std::endl;
        }

        if (racket_param.width > RACKET_DEFAULT_WIDTH)
        {

            racket.setParameters(racket_param.width - DEFAULT_RACKET_INCREASE, racket_param.height, racket_param.speed);
        }

        if (!ball_bounce_)
        {
            ball.reset();
            release_ball_ = false;
            ball_bounce_ = true;
        }
    }
}

bool GameControl::powerUpHitRacket(PowerUps &power_up, Racket &racket)
{

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

void GameControl::handlePowerUps(Brick &brick, Racket &racket, Ball &ball, GameStats &stats, Balls& balls)
{

    PowerUps &power_up = brick.getPowerUp();
    const float ball_speed = ball.getParameters().speed;
    const BallPositions& ball_pos = ball.getPositions();
    const BallDirection& ball_dir = ball.getDirection();
    const RacketParameters &racket_param = racket.getParameters();

    if (power_interruption_)
    {
        power_up.setPositions(-1, -1); // powers aren't usable when interruption
    }

    if (power_up.getType() != PowerType::NONE)
    {

        power_up.update();

        if (powerUpHitRacket(power_up, racket))
        {

            if (power_up.getType() == PowerType::PLAYER)
            {

                // We need to reset existing power up
                racket.reset();
                ball.reset(false);
                ball_bounce_ = true;
                release_ball_ = true;
                laser_on_ = false;

                stats.gainLife();
                std::cout << "[GAME CONTROL] you pick extra life" << std::endl;

            }
            else if (power_up.getType() == PowerType::CATCH_BALL)
            {

                racket.reset();
                ball_bounce_ = false;
                laser_on_ = false;

                std::cout << "[GAME CONTROL] you pick catch ball" << std::endl;

            }
            else if (power_up.getType() == PowerType::INTERRUPTION)
            {
                racket.reset();
                ball.reset(false);
                ball_bounce_ = true;
                release_ball_ = true;
                laser_on_ = false;

                power_interruption_ = true;
                for (int i=0; i < DEFAULT_NUMER_OF_EXTRA_BALLS; i++){
                    balls.createBall(ball_pos.x+i*2, ball_pos.y, ball_dir.x, ball_dir.y);
                }

                std::cout << "[GAME CONTROL] you pick interruption" << std::endl;
            }
            else if (power_up.getType() == PowerType::LASER)
            {

                racket.reset();
                ball.reset(false);
                ball_bounce_ = true;
                release_ball_ = true;
                laser_counter_ = DEFAULT_NUMBER_OF_LASER;
                laser_on_ = true;

                std::cout << "[GAME CONTROL] you pick laser" << std::endl;
            }
            else if (power_up.getType() == PowerType::RACKET_GROW)
            {

                ball.reset(false);
                ball_bounce_ = true;
                release_ball_ = true;
                laser_on_ = false;

                std::cout << "[GAME CONTROL] you pick racket grow" << std::endl;
                racket.setParameters(racket_param.width + DEFAULT_RACKET_INCREASE * 3, racket_param.height, racket_param.speed);
            }
            else if (power_up.getType() == PowerType::SLOW_DOWN)
            {

                racket.reset();
                ball_bounce_ = true;
                release_ball_ = true;
                laser_on_ = false;
                std::cout << "[GAME CONTROL] you pick slow down" << std::endl;
                if (ball_speed > 2)
                { // limit to 1.5 speed because 2.5 - 1 = 1.5
                    ball.setBallSpeed(ball_speed - 1);
                    std::cout << "[GAME CONTROL] ball is now slowed down: " << ball_speed - 1 << std::endl;
                }
            }
            power_up.disappear();
        }
    }
}

void GameControl::checkBrickCollisions(Balls &balls, Board &board, GameStats &stats, Racket &racket, Lasers& lasers)
{

    const int board_height = board.getParameters().height;
    const int board_width = board.getParameters().width;
    auto &bricks = board.getBricks();

    bool collision_happened = false;
    bool horizontal_collision = false;
    bool vertical_collision = false;

    // Find the closest brick that the ball collides with
    for (int r = 0; r < board_height; r++)
    {
        for (int c = 0; c < board_width; c++)
        {
            Brick &brick = bricks[r][c];


            const int brick_width = brick.getBrickType().width;
            const int brick_height = brick.getBrickType().height;

            // Brick boundaries
            const float brick_left = c * brick_width;
            const float brick_right = brick_left + brick_width;
            const float brick_top = r * brick_height + SEPARATION_LINE_HEIGHT;
            const float brick_bottom = brick_top + brick_height;

            for (auto* laser : lasers.getLasers()) {
                if (brick.isDestroyed()) {
                    continue;
                }

                const float laser_x = laser->getPositions().x1;
                const float laser_y = laser->getPositions().y1;


                if (laser_x >= brick_left && laser_x <= brick_right &&
                    laser_y >= brick_top && laser_y <= brick_bottom) {
                    
                    const int brick_points = brick.getBrickType().gained_points;
                    const float brick_middle_x = brick_left + ((brick_right - brick_left) / 2);
                    const float brick_middle_y = brick_top + ((brick_bottom - brick_top) / 2);
                    brick.hit(brick_middle_x, brick_middle_y);
                    stats.addScore(brick_points);
                    break; // when one laser hits a brick, don't need to check if other lasers hit the same brick
                }
            }


            for (auto& ball: balls.getBalls()){
                const BallPositions ball_pos = ball->getPositions();
                const float ball_radius = ball->getParameters().radius;
                const float ball_speed = ball->getParameters().speed;
                const BallDirection &direction = ball->getDirection();

                // Calculate next position based on current direction
                float next_x = ball_pos.x + direction.x * ball_speed;
                float next_y = ball_pos.y + direction.y * ball_speed;

                if (brick.isDestroyed() || collision_happened)
                    {
                        handlePowerUps(brick, racket, *ball, stats, balls);
                        continue; // we don't want collision with destroyed bricks
                    }

                // Check if ball overlaps with brick
                if (next_x + ball_radius >= brick_left && next_x - ball_radius <= brick_right &&
                    next_y + ball_radius >= brick_top && next_y - ball_radius <= brick_bottom)
                    {

                    // Vertical collision (top/bottom)
                    if (ball_pos.x + ball_radius >= brick_left && ball_pos.x - ball_radius <= brick_right)
                    {
                        if ((direction.y > 0 && ball_pos.y + ball_radius <= brick_top) ||
                            (direction.y < 0 && ball_pos.y - ball_radius >= brick_bottom))
                        {
                            vertical_collision = true;
                        }
                    }

                    // Horizontal collision (left/right)
                    if (ball_pos.y + ball_radius >= brick_top && ball_pos.y - ball_radius <= brick_bottom)
                    {
                        if ((direction.x > 0 && ball_pos.x + ball_radius <= brick_left) ||
                            (direction.x < 0 && ball_pos.x - ball_radius >= brick_right))
                        {
                            horizontal_collision = true;
                        }
                    }

                    // Corner collision
                    if (!vertical_collision && !horizontal_collision)
                    {
                        // Check which corner is closest to the ball
                        const float closest_x = std::max(brick_left, std::min(ball_pos.x, brick_right));
                        const float closest_y = std::max(brick_top, std::min(ball_pos.y, brick_bottom));

                        const float distance_x = ball_pos.x - closest_x;
                        const float distance_y = ball_pos.y - closest_y;

                        // If we have a corner hit
                        if (distance_x * distance_x + distance_y * distance_y <= ball_radius * ball_radius)
                        {
                            horizontal_collision = true;
                            vertical_collision = true;
                        }
                    }

                    // Process collision if detected
                    if (horizontal_collision || vertical_collision)
                    {
                        const int brick_points = brick.getBrickType().gained_points;
                        const float brick_middle_x = brick_left + ((brick_right - brick_left) / 2);
                        const float brick_middle_y = brick_top + ((brick_bottom - brick_top) / 2);
                        brick.hit(brick_middle_x, brick_middle_y);
                        stats.addScore(brick_points);

                        // Update ball direction
                        const float new_dx = horizontal_collision ? -direction.x : direction.x;
                        const float new_dy = vertical_collision ? -direction.y : direction.y;
                        ball->setDirection(new_dx, new_dy);

                        collision_happened = true;
                        break;
                    }
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

void GameControl::handleBallLost(GameStats &stats, Racket &racket, Board &board, Balls& balls, Ball& ball, Lasers& lasers)
{

    const int lives = stats.getBasicInfos().lives;

    if(balls.getBalls().size() > 1){
        balls.removeBall(ball);
        if(balls.getBalls().size()  == 1){
            std::cout << "[INTERRUPTION] Power OFF" << std::endl;
            power_interruption_ = false;
        }

    } else if (lives > 1)
    {
        stats.loseLife();
        ball.setDirection(BALL_DEFAULT_DX, BALL_DEFAULT_DY);
        ball.setPosition(racket.getPositions().x + (racket.getParameters().width / 2));
        release_ball_ = false;
    }
    else
    {
        std::cout << "[GAME CONTROL] GAME OVER" << std::endl;
        saveBestScore(stats, board);
        resetGame(stats, balls, racket, board, lasers);
        stats.setGameOverFlag(true);
        CURRENT_GAME_STATE = GameStates::END_GAME;
        release_ball_ = false;

    }
}

void GameControl::resetGame(GameStats &stats, Balls &balls, Racket &racket, Board &board, Lasers& lasers)
{
    stats.reset();
    racket.reset();
    board.reset();
    balls.reset();
    lasers.reset();
    game_has_started_ = false;
    release_ball_ = false;
    power_interruption_ = false;
    laser_on_ = false;
}

void GameControl::saveBestScore(GameStats &stats, Board &board)
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