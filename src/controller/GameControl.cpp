#include "controller/GameControl.hpp"
#include <string>




void GameControl::processInputs(Racket &racket, Ball &ball){

    if (CURRENT_GAME_STATE == GameStates::MAIN_MENU || CURRENT_GAME_STATE == GameStates::END_GAME){
        processMenuInput();
    } else if (CURRENT_GAME_STATE == GameStates::IN_GAME){
        processGameInput(racket, ball);
    }
}



void GameControl::processGameInput(Racket &racket, Ball &ball)
{
    al_get_keyboard_state(&ks_);
    const float racket_x = racket.getPositions().x;
    const int racket_speed = racket.getParameters().speed;
    const int racket_width = racket.getParameters().width;

    //LATER FOR INPUT OPTION BETWEEN MOUSE AND KEYBOARD
    ALLEGRO_MOUSE_STATE ms;
    al_get_mouse_state(&ms);
    // Mouse control
    int new_racket_x = ms.x - racket_width / 2;

    if (new_racket_x < 0) {
        new_racket_x = 0;
    }
    if (new_racket_x + racket_width > SCREEN_WIDTH) {
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
}



void GameControl::update(Board &board,
                         Ball &ball,
                         Racket &racket,
                         GameStats &stats)
{
        // Update ball position
        ball.update();
        checkBrickCollisions(ball, board, stats);
        checkWallCollisions(ball);
        checkRacketCollisions(ball, racket);

        if (ball_launched_ && ball.isLost())
        {
            handleBallLost(stats, ball, racket, board);
        }

        if (hasWon(board)){
            stats.setGameOverFlag(false);
            resetGame(stats, ball, racket, board);
            CURRENT_GAME_STATE = GameStates::END_GAME;
            saveBestScore(stats);
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
    if (ball_x + ball_radius  + ball_speed >= SCREEN_WIDTH || ball_x - ball_radius - ball_speed <= 0)
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
    const BallDirection &direction = ball.getDirection();
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
        ((ball_x + ball_radius >= racket_x) &&
             (ball_x <= racket_x + racket_width) ||
         ((ball_x >= racket_x) &&
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

// TODO REFACTO
void GameControl::checkBrickCollisions(Ball &ball, Board &board, GameStats &stats)
{

    const BallDirection &direction = ball.getDirection();
    const int board_height = board.getParameters().height;
    const int board_width = board.getParameters().width;
    auto &bricks = board.getBricks();
    const float ball_x = ball.getPositions().x;
    const float ball_y = ball.getPositions().y;
    const float ball_radius = ball.getParameters().radius;
    bool did_it_already_hit = false; // one brick destroyed at a time

    for (int r = 0; r < board_height; r++)
    {

        for (int c = 0; c < board_width; c++)
        {

            Brick &brick = bricks[r][c];
            const int brick_width = brick.getBrickType().width;
            const int brick_height = brick.getBrickType().height;

            // brick collision; 4 cases; bottom, top, right, left

            if (!brick.isDestroyed() && !did_it_already_hit)
            {

                if (ball_x >= c * brick_width && ball_x <= c * brick_width + brick_width

                    && ((ball_y - ball_radius > r * brick_height + SEPARATION_LINE_HEIGHT && // check hit from bottom
                         ball_y - ball_radius < r * brick_height + brick_height + SEPARATION_LINE_HEIGHT)

                        || (ball_y + ball_radius > r * brick_height + SEPARATION_LINE_HEIGHT && // check hit from top
                            ball_y + ball_radius < r * brick_height + brick_height + SEPARATION_LINE_HEIGHT)))
                {
                    // std::cout << "[GAME CONTROL] BRICK HIT FROM TOP OR BOTTOM" << std::endl;
                    const int brick_points = brick.getBrickType().gained_points;
                    brick.hit();
                    stats.addScore(brick_points);
                    ball.setDirection(direction.x, -direction.y);
                    did_it_already_hit = true;
                }

                else if (ball_y > r * brick_height + SEPARATION_LINE_HEIGHT &&
                         ball_y <= r * brick_height + brick_height + SEPARATION_LINE_HEIGHT

                         && ((ball_x + ball_radius > c * brick_width && ball_x + ball_radius < c * brick_width + brick_width) // check hit from right

                             || (ball_x - ball_radius > c * brick_width && ball_x - ball_radius < c * brick_width + brick_width)) // check hit from left
                )
                {
                    const int brick_points = brick.getBrickType().gained_points;
                    brick.hit();
                    stats.addScore(brick_points);
                    ball.setDirection(-direction.x, direction.y);
                    did_it_already_hit = true;
                }
            }
        }
    }
}


bool GameControl::hasWon(Board& board){


    const int board_height = board.getParameters().height;
    const int board_width = board.getParameters().width;
    auto &bricks = board.getBricks();

    for (int r = 0; r < board_height; r++){

        for (int c = 0; c < board_width; c++){

            Brick &brick = bricks[r][c];
            if(!brick.isDestroyed())
                {return false;}
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
        ball.setDirection(0,-ball_speed);
    }
    else
    {
        saveBestScore(stats);
        resetGame(stats, ball, racket, board);
        stats.setGameOverFlag(true);
        CURRENT_GAME_STATE = GameStates::END_GAME;

    }
    ball_launched_ = false;
}

void GameControl::resetGame(GameStats &stats, Ball &ball, Racket &racket, Board &board){
    stats.reset();
    racket.reset();
    ball.reset();
    board.reset();
    ball_launched_ = false;
}

void GameControl::saveBestScore(GameStats& stats){

    const int best_score = loadScore(SCORE_PATH);
    const int current_score = stats.getBasicInfos().score;
    std::cout << best_score << "  " << current_score << std::endl;
    if (best_score < current_score){
        
        saveScore(current_score, SCORE_PATH);
    }
}