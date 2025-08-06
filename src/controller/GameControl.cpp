#include "controller/GameControl.hpp"
#include <string>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>

// =================================================================================================
// SECTION: Main Game Loop Methods
// =================================================================================================

/**
 * @brief Processes user inputs and updates the game state based on the current phase of the game.
 * @param racket The player's racket.
 * @param balls The collection of balls in the game.
 * @param board The game board containing bricks.
 * @param stats The current game statistics (score, lives, etc.).
 * @param lasers The collection of lasers shot by the racket.
 */
void GameControl::processInputs(Racket &racket, Balls &balls, Board &board, GameStats &stats, Lasers &lasers)
{
    if (CURRENT_GAME_STATE == GameStates::WELCOME)
    {
        processWelcomeInput();
    }
    else if (CURRENT_GAME_STATE == GameStates::IN_GAME)
    {
        if (!game_has_started_)
        {
            // Setting default ball, positions and direction
            balls.createBall();
            std::unique_ptr<Ball> &ball = balls.getBalls()[0];
            ball->setPosition(racket.getPositions().x + (racket.getParameters().width / 2));
            ball->setDirection(BALL_DEFAULT_DX, BALL_DEFAULT_DY);
            game_has_started_ = true;
        }
        else
        {
            update(board, racket, stats, balls, lasers);
            processGameInput(stats, racket, balls, lasers, board);
        }
    }
    else if (CURRENT_GAME_STATE == GameStates::END_GAME)
    {
        processEndGameInput();
    }
}

/**
 * @brief Updates the state of all game objects (balls, lasers) and checks for collisions.
 * @param board The game board.
 * @param racket The player's racket.
 * @param stats The game statistics.
 * @param balls The collection of balls.
 * @param lasers The collection of lasers.
 */
void GameControl::update(Board &board, Racket &racket, GameStats &stats, Balls &balls, Lasers &lasers)
{
    std::vector<Ball *> lost_ball; // temporary vector of pointer in stack
    std::vector<Laser *> lost_laser;

    for (auto &ball : balls.getBalls())
    {
        if (!ball->isLost())
        {
            ball->update();
            checkWallCollisions(*ball);
            checkRacketCollisions(*ball, racket);
        }
        else
        {
            lost_ball.push_back(ball.get());
        }
    }

    for (auto &laser : lasers.getLasers())
    {
        if (!laser->isLost())
        {
            laser->update();
        }
        else
        {
            lost_laser.push_back(laser.get());
        }
    }

    // Handle lost balls and lasers after iterating to avoid invalidating iterators
    for (auto *ball : lost_ball)
    {
        handleBallLost(stats, racket, board, balls, *ball, lasers);
    }

    for (auto *laser : lost_laser)
    {
        lasers.removeLaser(*laser);
    }

    checkBrickCollisions(balls, board, stats, racket, lasers);

    if (hasWon(board))
    {
        stats.setGameOverFlag(false);
        saveBestScore(stats, board);
        resetGame(stats, balls, racket, board, lasers);
        CURRENT_GAME_STATE = GameStates::END_GAME;
        int new_level_number = (board.getLevelNumber() + 1) % TOTAL_LEVELS;
        std::vector<std::vector<Brick>> new_level = loadLevel(LEVEL_PATH_MAP[new_level_number]);
        board.changeLevel(new_level, new_level_number);
    }
}

// =================================================================================================
// SECTION: Input Processing Methods
// =================================================================================================

/**
 * @brief Processes user inputs during the IN_GAME state.
 * @param stats The game statistics.
 * @param racket The player's racket.
 * @param balls The collection of balls.
 * @param lasers The collection of lasers.
 * @param board The game board.
 */
void GameControl::processGameInput(GameStats &stats, Racket &racket, Balls &balls, Lasers &lasers, Board &board)
{
    al_get_keyboard_state(&ks_);
    const float racket_x = racket.getPositions().x;
    const int racket_width = racket.getParameters().width;

    checkChangeInputControl(racket);
    checkDebugInputs(board, stats, lasers, balls, racket);

    if (!release_ball_)
    {
        for (auto &ball : balls.getBalls())
        {
            const float ball_x = ball->getPositions().x;
            const float delta = ball_x - (racket_x + (racket_width / 2));
            ball->setPosition(racket.getPositions().x + (racket_width / 2) + delta);
        }
    }

    if (al_key_down(&ks_, ALLEGRO_KEY_SPACE))
    {
        if (!space_pressed_)
        {
            release_ball_ = true;
            if (laser_on_)
            {
                lasers.createLaser(racket.getPositions().x + racket_width / 2);
                lasers.setRemainingLaser(lasers.getRemainingLaser() - 1);
                if (lasers.getRemainingLaser() == 0)
                {
                    lasers.setRemainingLaser(DEFAULT_NUMBER_OF_LASER);
                    laser_on_ = false;
                }
            }
        }
        space_pressed_ = true;
    }
    else
    {
        space_pressed_ = false;
    }

    if (al_key_down(&ks_, ALLEGRO_KEY_L))
    {
        running_flag_ = false;
        resetGame(stats, balls, racket, board, lasers);
    }
}

/**
 * @brief Processes input on the welcome screen to start the game.
 */
void GameControl::processWelcomeInput()
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

/**
 * @brief Processes input on the end game screen to restart the game.
 */
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

/**
 * @brief Checks for and handles racket movement input (mouse or keyboard).
 * @param racket The player's racket.
 */
void GameControl::checkChangeInputControl(Racket &racket)
{
    const float racket_x = racket.getPositions().x;
    const int racket_speed = racket.getParameters().speed;
    const int racket_width = racket.getParameters().width;

    if (mouse_control_)
    {
        ALLEGRO_MOUSE_STATE ms;
        al_get_mouse_state(&ms);
        int new_racket_x = ms.x - racket_width / 2;

        if (new_racket_x < 0)
            new_racket_x = 0;
        if (new_racket_x + racket_width > SCREEN_WIDTH)
            new_racket_x = SCREEN_WIDTH - racket_width;
        racket.setPosition(new_racket_x);
    }
    else
    {
        if (al_key_down(&ks_, ALLEGRO_KEY_LEFT))
        {
            if (racket_x - racket_speed >= 0)
                racket.moveLeft();
        }
        if (al_key_down(&ks_, ALLEGRO_KEY_RIGHT))
        {
            if (racket_x + racket_width + racket_speed <= SCREEN_WIDTH)
                racket.moveRight();
        }
    }
}

/**
 * @brief Checks for and handles debug-related inputs (e.g., changing levels, resetting score).
 * @param board The game board.
 * @param stats The game statistics.
 * @param lasers The collection of lasers.
 * @param balls The collection of balls.
 * @param racket The player's racket.
 */
void GameControl::checkDebugInputs(Board &board, GameStats &stats, Lasers &lasers, Balls &balls, Racket &racket)
{
    if (al_key_down(&ks_, ALLEGRO_KEY_K))
    {
        std::cout << "[GAME CONTROL] switching to keyboard/mouse for racket" << std::endl;
        mouse_control_ = !mouse_control_;
    }

    if (al_key_down(&ks_, ALLEGRO_KEY_R))
    {
        int score_level = board.getLevelNumber();
        saveScore(0, BEST_SCORE_PATH_MAP[score_level]);
        std::cout << "[GAME CONTROL] reset best score" << std::endl;
    }

    for (int i = ALLEGRO_KEY_0; i <= ALLEGRO_KEY_3; ++i)
    {
        if (al_key_down(&ks_, i))
        {
            int level_num = i - ALLEGRO_KEY_0;
            resetGame(stats, balls, racket, board, lasers);
            std::vector<std::vector<Brick>> new_level = loadLevel(LEVEL_PATH_MAP[level_num]);
            board.changeLevel(new_level, level_num);
            std::cout << "[GAME CONTROL] changed to level " << level_num << std::endl;
        }
    }
}

// =================================================================================================
// SECTION: Collision Detection and Handling
// =================================================================================================

/**
 * @brief Checks for and handles collisions between the ball and the screen walls.
 * @param ball The ball to check.
 */
void GameControl::checkWallCollisions(Ball &ball)
{
    const BallDirection &direction = ball.getDirection();
    const BallPositions &ball_pos = ball.getPositions();
    const float ball_radius = ball.getParameters().radius;

    if (ball_pos.x + ball_radius >= SCREEN_WIDTH || ball_pos.x - ball_radius <= 0)
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

/**
 * @brief Checks for and handles collisions between the ball and the racket.
 * @param ball The ball to check.
 * @param racket The player's racket.
 */
void GameControl::checkRacketCollisions(Ball &ball, Racket &racket)
{
    const BallPositions ball_pos = ball.getPositions();
    float ball_speed = ball.getParameters().speed;
    const float ball_radius = ball.getParameters().radius;
    const RacketPositions &racket_pos = racket.getPositions();
    const RacketParameters &racket_param = racket.getParameters();

    if ((ball_pos.y + ball_radius >= racket_pos.y) &&
        (ball_pos.y + ball_radius <= racket_pos.y + racket_param.height) &&
        (ball_pos.x >= racket_pos.x && ball_pos.x <= racket_pos.x + racket_param.width))
    {
        float racket_hit_x = ball_pos.x - racket_pos.x;
        const int racket_middle = RACKET_DEFAULT_WIDTH / 2;
        float new_dx, new_dy;

        if (racket_hit_x == racket_middle)
        {
            new_dy = BALL_DEFAULT_DY;
            new_dx = BALL_DEFAULT_DX;
        }
        else
        {
            float angle_rad;
            if (racket_hit_x < racket_middle)
            {
                angle_rad = (30 + 120 * (1 - (racket_hit_x / racket_param.width))) * M_PI / 180.0;
                new_dy = -sin(angle_rad);
                new_dx = cos(angle_rad);
            }
            else
            {
                racket_hit_x = racket_param.width - racket_hit_x;
                angle_rad = (30 + 120 * (1 - (racket_hit_x / racket_param.width))) * M_PI / 180.0;
                new_dy = -sin(angle_rad);
                new_dx = -cos(angle_rad);
            }
        }
        ball.setDirection(new_dx, new_dy);

        if (ball_speed < BALL_DEFAULT_SPEED)
        {
            ball.setBallSpeed(std::min(ball_speed + 0.5f, (float)BALL_DEFAULT_SPEED));
            std::cout << "[GAME CONTROL] ball is increasing speed: " << ball.getParameters().speed << std::endl;
        }

        if (racket_param.width > RACKET_DEFAULT_WIDTH)
        {
            racket.setParameters(racket_param.width - DEFAULT_RACKET_INCREASE, racket_param.height, racket_param.speed);
        }

        if (!ball_bounce_)
        {
            if (ball_pos.x + ball_radius >= racket_pos.x + racket_param.width)
            {
                ball.setPosition(racket_pos.x + racket_param.width - ball_radius);
            }
            else if (ball_pos.x - ball_radius <= racket_pos.x)
            {
                ball.setPosition(racket_pos.x + ball_radius);
            }
            ball.reset();
            release_ball_ = false;
            ball_bounce_ = true;
        }
    }
}

/**
 * @brief Checks for collisions between balls, lasers, and bricks.
 * @param balls The collection of balls.
 * @param board The game board.
 * @param stats The game statistics.
 * @param racket The player's racket.
 * @param lasers The collection of lasers.
 */
void GameControl::checkBrickCollisions(Balls &balls, Board &board, GameStats &stats, Racket &racket, Lasers &lasers)
{
    const int board_height = board.getParameters().height;
    const int board_width = board.getParameters().width;
    auto &bricks = board.getBricks();

    for (int r = 0; r < board_height; r++)
    {
        for (int c = 0; c < board_width; c++)
        {
            Brick &brick = bricks[r][c];
            checkLaserCollisions(brick, lasers, stats, c, r);
            checkBallCollisions(balls, racket, brick, lasers, stats, false, false, c, r);
        }
    }
}

/**
 * @brief Checks for collisions between lasers and a single brick.
 * @param brick The brick to check against.
 * @param lasers The collection of lasers.
 * @param stats The game statistics.
 * @param c The column index of the brick.
 * @param r The row index of the brick.
 */
void GameControl::checkLaserCollisions(Brick &brick, Lasers &lasers, GameStats &stats, int c, int r)
{
    if (brick.isDestroyed())
        return;

    const int brick_width = brick.getBrickType().width;
    const int brick_height = brick.getBrickType().height;
    const float brick_left = c * brick_width;
    const float brick_right = brick_left + brick_width;
    const float brick_top = r * brick_height + SEPARATION_LINE_HEIGHT;
    const float brick_bottom = brick_top + brick_height;

    for (auto &laser : lasers.getLasers())
    {
        const float laser_x = laser->getPositions().x1;
        const float laser_y = laser->getPositions().y1;

        if (laser_x >= brick_left && laser_x <= brick_right &&
            laser_y >= brick_top && laser_y <= brick_bottom)
        {

            if (brick.getBrickType().color == BrickColor::GOLD)
            {
                lasers.removeLaser(*laser);
            }

            const int brick_points = brick.getBrickType().gained_points;
            const float brick_middle_x = brick_left + (brick_width / 2.0f);
            const float brick_middle_y = brick_top + (brick_height / 2.0f);
            brick.hit(brick_middle_x, brick_middle_y, power_interruption_);
            stats.addScore(brick_points);
            break; // no need to check other lasers
        }
    }
}

/**
 * @brief Checks for collisions between balls and a single brick.
 * @param balls The collection of balls.
 * @param racket The player's racket.
 * @param brick The brick to check against.
 * @param lasers The collection of lasers.
 * @param stats The game statistics.
 * @param vertical_collision Flag for vertical collision.
 * @param horizontal_collision Flag for horizontal collision.
 * @param collision_happened Flag indicating if a collision occurred in this frame.
 * @param c The column index of the brick.
 * @param r The row index of the brick.
 */
void GameControl::checkBallCollisions(Balls &balls, Racket &racket, Brick &brick, Lasers &lasers, GameStats &stats, bool vertical_collision, bool horizontal_collision, int c, int r)
{

    const int brick_width = brick.getBrickType().width;
    const int brick_height = brick.getBrickType().height;
    const float brick_left = c * brick_width;
    const float brick_right = brick_left + brick_width;
    const float brick_top = r * brick_height + SEPARATION_LINE_HEIGHT;
    const float brick_bottom = brick_top + brick_height;

    for (auto &ball : balls.getBalls())
    {
        const BallPositions ball_pos = ball->getPositions();
        const float ball_radius = ball->getParameters().radius;
        const float ball_speed = ball->getParameters().speed;
        const BallDirection &direction = ball->getDirection();

        if (brick.isDestroyed())
        {
            handlePowerUps(brick, racket, *ball, stats, balls, lasers);
            return;
        }

        float next_x = ball_pos.x + direction.x * ball_speed;
        float next_y = ball_pos.y + direction.y * ball_speed;

        // AABB COLLISION ALGORITHM, checking vertical, horizontal and corner collisions

        if (next_x + ball_radius >= brick_left && next_x - ball_radius <= brick_right &&
            next_y + ball_radius >= brick_top && next_y - ball_radius <= brick_bottom)
        {
            if (ball_pos.x + ball_radius >= brick_left && ball_pos.x - ball_radius <= brick_right)
            {
                if ((direction.y > 0 && ball_pos.y + ball_radius <= brick_top) || (direction.y < 0 && ball_pos.y - ball_radius >= brick_bottom))
                {
                    vertical_collision = true;
                }
            }
            if (ball_pos.y + ball_radius >= brick_top && ball_pos.y - ball_radius <= brick_bottom)
            {
                if ((direction.x > 0 && ball_pos.x + ball_radius <= brick_left) || (direction.x < 0 && ball_pos.x - ball_radius >= brick_right))
                {
                    horizontal_collision = true;
                }
            }

            // Interesting algorithm to detect collision, using pythagore to check if nearest distance between brick and ball is less or equal to ball radius
            if (!vertical_collision && !horizontal_collision)
            {
                const float closest_x = std::max(brick_left, std::min(ball_pos.x, brick_right));
                const float closest_y = std::max(brick_top, std::min(ball_pos.y, brick_bottom));
                const float distance_x = ball_pos.x - closest_x;
                const float distance_y = ball_pos.y - closest_y;
                if (distance_x * distance_x + distance_y * distance_y <= ball_radius * ball_radius)
                {
                    horizontal_collision = true;
                    vertical_collision = true;
                }
            }

            if (horizontal_collision || vertical_collision)
            {
                const int brick_points = brick.getBrickType().gained_points;
                const float brick_middle_x = brick_left + (brick_width / 2.0f);
                const float brick_middle_y = brick_top + (brick_height / 2.0f);
                brick.hit(brick_middle_x, brick_middle_y, power_interruption_);
                stats.addScore(brick_points);

                const float new_dx = horizontal_collision ? -direction.x : direction.x;
                const float new_dy = vertical_collision ? -direction.y : direction.y;
                ball->setDirection(new_dx, new_dy);
                break;
            }
        }
    }
}

// =================================================================================================
// SECTION: Power-Up Logic
// =================================================================================================

/**
 * @brief Checks if a falling power-up has been caught by the racket.
 * @param power_up The power-up to check.
 * @param racket The player's racket.
 * @return True if the power-up hits the racket, false otherwise.
 */
bool GameControl::powerUpHitRacket(PowerUps &power_up, Racket &racket)
{
    const float power_up_x = power_up.getPositions().x;
    const float power_up_y = power_up.getPositions().y;
    const float power_up_radius = power_up.getParameters().radius;
    const float racket_x = racket.getPositions().x;
    const float racket_y = racket.getPositions().y;
    const int racket_height = racket.getParameters().height;
    const int racket_width = racket.getParameters().width;

    return (power_up_y + power_up_radius >= racket_y) &&
           (power_up_y + power_up_radius <= racket_y + racket_height) &&
           (power_up_x + power_up_radius >= racket_x) &&
           (power_up_x - power_up_radius <= racket_x + racket_width);
}

/**
 * @brief Handles the logic for falling power-ups, including activation when caught by the racket.
 * @param brick The brick that released the power-up.
 * @param racket The player's racket.
 * @param ball The main ball.
 * @param stats The game statistics.
 * @param balls The collection of all balls.
 * @param lasers The collection of lasers.
 */
void GameControl::handlePowerUps(Brick &brick, Racket &racket, Ball &ball, GameStats &stats, Balls &balls, Lasers &lasers)
{
    PowerUps &power_up = brick.getPowerUp();
    if (power_interruption_ || power_up.getType() == PowerType::NONE)
    {
        return;
    }

    power_up.update();

    if (powerUpHitRacket(power_up, racket))
    {
        // Reset any existing temporary power-ups
        racket.reset();
        ball_bounce_ = true;
        laser_on_ = false;
        release_ball_ = true;
        lasers.reset();

        switch (power_up.getType())
        {
        case PowerType::PLAYER:
            stats.gainLife();
            ball.reset(false); // need to reset ball speed, its stackable
            std::cout << "[GAME CONTROL] you pick extra life" << std::endl;
            break;
        case PowerType::CATCH_BALL:
            ball.reset(false);
            ball_bounce_ = false;
            std::cout << "[GAME CONTROL] you pick catch ball" << std::endl;
            break;
        case PowerType::INTERRUPTION:
            ball.reset(false);
            power_interruption_ = true;
            for (int i = 1; i < DEFAULT_NUMER_OF_BALLS; i++)
            {
                balls.createBall(ball.getPositions().x + i * 2, ball.getPositions().y, ball.getDirection().x, ball.getDirection().y);
            }
            std::cout << "[GAME CONTROL] you pick interruption" << std::endl;
            break;
        case PowerType::LASER:
            ball.reset(false);
            laser_on_ = true;
            std::cout << "[GAME CONTROL] you pick laser" << std::endl;
            break;
        case PowerType::RACKET_GROW:
            ball.reset(false);
            racket.setParameters(racket.getParameters().width + DEFAULT_RACKET_INCREASE * 3, racket.getParameters().height, racket.getParameters().speed);
            std::cout << "[GAME CONTROL] you pick racket grow" << std::endl;
            break;
        case PowerType::SLOW_DOWN:
            if (ball.getParameters().speed > 2)
            {
                ball.setBallSpeed(ball.getParameters().speed - 1);
                std::cout << "[GAME CONTROL] ball is now slowed down: " << ball.getParameters().speed << std::endl;
            }
            break;
        default:
            break;
        }
        power_up.destroy();
    }
}

// =================================================================================================
// SECTION: Game State Management
// =================================================================================================

/**
 * @brief Checks if the player has won the level by destroying all non-gold bricks.
 * @param board The game board.
 * @return True if the level is won, false otherwise.
 */
bool GameControl::hasWon(Board &board)
{
    auto &bricks = board.getBricks();
    for (const auto &row : bricks)
    {
        for (const auto &brick : row)
        {
            if (!brick.isDestroyed() && brick.getBrickType().color != BrickColor::GOLD)
            {
                return false;
            }
        }
    }
    return true;
}

/**
 * @brief Checks if the game is currently running.
 * @return True if the game is running, false otherwise.
 */
bool GameControl::isRunning() const
{
    return running_flag_;
}

/**
 * @brief Handles the event of a ball being lost (going off-screen).
 * @param stats The game statistics.
 * @param racket The player's racket.
 * @param board The game board.
 * @param balls The collection of balls.
 * @param ball The specific ball that was lost.
 * @param lasers The collection of lasers.
 */
void GameControl::handleBallLost(GameStats &stats, Racket &racket, Board &board, Balls &balls, Ball &ball, Lasers &lasers)
{
    if (balls.getBalls().size() > 1)
    {
        balls.removeBall(ball);
        if (balls.getBalls().size() == 1)
        {
            std::cout << "[INTERRUPTION] Power OFF" << std::endl;
            power_interruption_ = false;
        }
    }
    else if (stats.getBasicInfos().lives > 1)
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
        CURRENT_GAME_STATE = GameStates::END_GAME;
        stats.setGameOverFlag(true);
        resetGame(stats, balls, racket, board, lasers);
        release_ball_ = false;
    }
}

/**
 * @brief Resets the game state for a new game or level.
 * @param stats The game statistics.
 * @param balls The collection of balls.
 * @param racket The player's racket.
 * @param board The game board.
 * @param lasers The collection of lasers.
 */
void GameControl::resetGame(GameStats &stats, Balls &balls, Racket &racket, Board &board, Lasers &lasers)
{
    stats.reset();
    racket.reset();
    board.reset();
    balls.reset();
    lasers.reset();
    game_has_started_ = false;
    release_ball_ = false;
    ball_bounce_ = true;
    power_interruption_ = false;
    laser_on_ = false;
}

/**
 * @brief Saves the player's score if it's a new high score for the current level.
 * @param stats The game statistics.
 * @param board The game board.
 */
void GameControl::saveBestScore(GameStats &stats, Board &board)
{
    const int level_number = board.getLevelNumber();
    const int best_score = loadScore(BEST_SCORE_PATH_MAP.at(level_number));
    const int current_score = stats.getBasicInfos().score;

    if (best_score < current_score)
    {
        saveScore(current_score, BEST_SCORE_PATH_MAP.at(level_number));
    }
}