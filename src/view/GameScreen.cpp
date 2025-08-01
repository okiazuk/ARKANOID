#include "view/GameScreen.hpp"
#include "../utils/Configs.hpp"

/**
 * @brief everything should be const because we simply print from the model
 * model is using float and view is casting in int
 */

void GameScreen::draw(Board &board, Balls &balls, Racket &racket, GameStats &stats, Lasers& lasers)
{

    if (CURRENT_GAME_STATE == GameStates::IN_GAME)
    {
        drawInGame(board, balls, racket, stats, lasers);
    }
    else if (CURRENT_GAME_STATE == GameStates::END_GAME)
    {
        drawEndGame(stats);
    }
    else if (CURRENT_GAME_STATE == GameStates::WELCOME)
    {
        drawWelcome();
    }
}

void GameScreen::drawInGame(const Board &board, const Balls &balls, const Racket &racket, const GameStats &stats, const Lasers& lasers)
{
    al_clear_to_color(al_map_rgb(BLACKGROUND_VEC[0], BLACKGROUND_VEC[1], BLACKGROUND_VEC[2]));

    // Draw bricks
    const auto &bricks = board.getBricks();
    const int board_height = board.getParameters().height;
    const int board_width = board.getParameters().width;
    for (int r = 0; r < board_height; ++r)
    {
        for (int c = 0; c < board_width; ++c)
        {
            const Brick &brick = bricks[r][c];
            const BrickColor &brick_color = brick.getBrickType().color;
            const int brick_width = brick.getBrickType().width;
            const int brick_height = brick.getBrickType().height;
            const std::vector<int> &brick_rgb = brick.getBrickType().rgb_values;

            const PowerUps &power_up = brick.getPowerUp();
            const PowerUpPositions power_up_pos = power_up.getPositions();
            const PowerType power_type = power_up.getType();
            const int power_up_pos_x = power_up_pos.x;
            const int power_up_pos_y = power_up_pos.y;
            const int power_up_speed = power_up.getParameters().speed;
            if (!brick.isDestroyed() && brick_color != BrickColor::NONE)
            {
                int x1 = c * brick_width;
                int y1 = r * brick_height + SEPARATION_LINE_HEIGHT;
                int x2 = x1 + brick_width;
                int y2 = y1 + brick_height;

                // Draw filled brick
                al_draw_filled_rectangle(x1, y1, x2, y2, al_map_rgb(brick_rgb[0], brick_rgb[1], brick_rgb[2]));

                // Draw brick outline
                al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(BLACKGROUND_VEC[0], BLACKGROUND_VEC[1], BLACKGROUND_VEC[2]), 2.0);

                // Draw a letter in the middle of the brick
                int center_x = x1 + brick_width / 2;
                int center_y = y1 + brick_height / 2;

                char letter_str[2];
                letter_str[0] = POWER_UP_TO_LETTER_MAP[static_cast<int>(power_type)];
                letter_str[1] = '\0';

                // Draw the letter centered in the brick
                al_draw_text(font_, al_map_rgb(BLACKGROUND_VEC[0], BLACKGROUND_VEC[1], BLACKGROUND_VEC[2]), center_x,
                             center_y - al_get_font_line_height(font_) / 2, ALLEGRO_ALIGN_CENTER, letter_str);

            }
            else
            {
                if (power_type != PowerType::NONE)
                {
                    al_draw_filled_circle(
                        power_up_pos_x,
                        power_up_pos_y,
                        power_up_speed,
                        al_map_rgb(PURPLE_UP_VEC[0], PURPLE_UP_VEC[1], PURPLE_UP_VEC[2]));
                }
            }
        }
    }

    // Draw racket
    const RacketPositions &racket_pos = racket.getPositions();
    const RacketParameters &racket_param = racket.getParameters();

    al_draw_filled_rectangle(
        (racket_pos.x), (racket_pos.y),
        (racket_pos.x) + racket_param.width, (racket_pos.y) + racket_param.height,
        al_map_rgb(WHITE_VEC[0], WHITE_VEC[1], WHITE_VEC[2]));

    // Draw ball with proper rounding
    for (auto& ball: balls.getBalls()) {
        if(!ball->isLost()){
            const BallPositions &ball_pos = ball->getPositions();
            al_draw_filled_circle(
                (ball_pos.x), // explicit conversion with rounding
                (ball_pos.y),
                (ball->getParameters().radius),
                al_map_rgb(WHITE_VEC[0], WHITE_VEC[1], WHITE_VEC[2]));
        }
    }

    // Draw lasers
    for (auto& laser : lasers.getLasers()) {
        const auto& pos = laser->getPositions();
        al_draw_filled_rectangle(
           (pos.x1),
            (pos.y1),
            (pos.x2),
            (pos.y2),
            al_map_rgb(RED_VEC[0], RED_VEC[1], RED_VEC[2]) // Red color for lasers
        );
    }

    drawUI(stats, board);

    al_flip_display();
}

void GameScreen::destroy()
{
    al_destroy_display(display_);
}

void GameScreen::drawUI(const GameStats &stats, const Board &board)
{

    // Draw lines
    al_draw_line(0, SEPARATION_LINE_HEIGHT, SCREEN_WIDTH, SEPARATION_LINE_HEIGHT,
                 al_map_rgb(WHITE_VEC[0], WHITE_VEC[0], WHITE_VEC[0]), 1.0);

    al_draw_line(0, 0, SCREEN_WIDTH, 0,
                 al_map_rgb(WHITE_VEC[0], WHITE_VEC[0], WHITE_VEC[0]), 2.0);

    al_draw_line(0, 0, 0, SCREEN_HEIGHT,
                 al_map_rgb(WHITE_VEC[0], WHITE_VEC[0], WHITE_VEC[0]), 2.0);

    al_draw_line(SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
                 al_map_rgb(WHITE_VEC[0], WHITE_VEC[0], WHITE_VEC[0]), 2.0);

    ALLEGRO_TRANSFORM transform;
    al_copy_transform(&transform, al_get_current_transform());

    // Scale text by 2x
    ALLEGRO_TRANSFORM scale_transform;
    al_identity_transform(&scale_transform);
    al_scale_transform(&scale_transform, 2.0, 2.0);
    al_use_transform(&scale_transform);

    // Print score left side
    char score_text[50];
    sprintf(score_text, "Score: %d", stats.getBasicInfos().score);
    al_draw_text(font_, al_map_rgb(WHITE_VEC[0], WHITE_VEC[0], WHITE_VEC[0]), UI_TEXT_INFO_WIDTH_BETWEEN_WALLS, UI_TEXT_INFO_HEIGHT,
                 0, score_text);

    const int level_number = board.getLevelNumber();
    // Print Best Score
    char best_score_text[50];
    sprintf(best_score_text, "Best Score: %d", loadScore(BEST_SCORE_PATH_MAP.at(level_number)));
    al_draw_text(font_, al_map_rgb(WHITE_VEC[0], WHITE_VEC[0], WHITE_VEC[0]), UI_TEXT_INFO_WIDTH_BETWEEN_WALLS * 7, UI_TEXT_INFO_HEIGHT,
                 0, best_score_text);

    // Print Level
    char level[50];
    sprintf(level, "Level: %d", board.getLevelNumber());
    al_draw_text(font_, al_map_rgb(WHITE_VEC[0], WHITE_VEC[0], WHITE_VEC[0]), UI_TEXT_INFO_WIDTH_BETWEEN_WALLS * 17, UI_TEXT_INFO_HEIGHT,
                 0, level);

    const int base_width = al_get_text_width(font_, "Lives:  ");
    const int scaled_width = base_width * 2.0;

    // Print score right side
    char lives_text[50];
    sprintf(lives_text, "Lives: %d", stats.getBasicInfos().lives);
    al_draw_text(font_, al_map_rgb(WHITE_VEC[0], WHITE_VEC[0], WHITE_VEC[0]), (SCREEN_WIDTH - scaled_width) / 2 - UI_TEXT_INFO_WIDTH_BETWEEN_WALLS,
                 UI_TEXT_INFO_HEIGHT, 0, lives_text);

    // Restore original transform
    al_use_transform(&transform);
}

void GameScreen::drawWelcome()
{

    al_clear_to_color(al_map_rgb(BLACKGROUND_VEC[0], BLACKGROUND_VEC[1], BLACKGROUND_VEC[2]));

    // Save current transform
    ALLEGRO_TRANSFORM transform;
    al_copy_transform(&transform, al_get_current_transform());

    // Create a larger scale for game over text
    ALLEGRO_TRANSFORM scale_transform;
    al_identity_transform(&scale_transform);
    al_scale_transform(&scale_transform, 3.0, 3.0);

    al_use_transform(&scale_transform);

    int base_width = al_get_text_width(font_, "WELCOME TO ARKANOID");
    int scaled_width = base_width * 3;

            al_draw_text(font_, al_map_rgb(WHITE_VEC[0], WHITE_VEC[1], WHITE_VEC[2]),
                     (SCREEN_WIDTH - scaled_width) / 6, (SCREEN_HEIGHT / 6), 0, "WELCOME TO ARKANOID");

        // Scale down a bit for other text
        al_identity_transform(&scale_transform);
        al_scale_transform(&scale_transform, 2.0, 2.0);
        al_use_transform(&scale_transform);

        base_width = al_get_text_width(font_, "Press any key to start");
        scaled_width = base_width * 2;
        al_draw_text(font_, al_map_rgb(WHITE_VEC[0], WHITE_VEC[1], WHITE_VEC[2]),
                     (SCREEN_WIDTH - scaled_width) / 4, (SCREEN_HEIGHT / 4) + UI_TEXT_INFO_HEIGHT * 2, 0, "Press any key to start");


    // Restore original transform
    al_use_transform(&transform);

    al_flip_display();


}

void GameScreen::drawEndGame(const GameStats &stats)
{

    bool game_over = stats.getBasicInfos().game_over;

    al_clear_to_color(al_map_rgb(BLACKGROUND_VEC[0], BLACKGROUND_VEC[1], BLACKGROUND_VEC[2]));

    // Save current transform
    ALLEGRO_TRANSFORM transform;
    al_copy_transform(&transform, al_get_current_transform());

    // Create a larger scale for game over text
    ALLEGRO_TRANSFORM scale_transform;
    al_identity_transform(&scale_transform);
    al_scale_transform(&scale_transform, 3.0, 3.0);

    al_use_transform(&scale_transform);

    if (game_over)
    {
        int base_width = al_get_text_width(font_, "GAME OVER");
        int scaled_width = base_width * 3;

        al_draw_text(font_, al_map_rgb(WHITE_VEC[0], WHITE_VEC[1], WHITE_VEC[2]),
                     (SCREEN_WIDTH - scaled_width) / 6, (SCREEN_HEIGHT / 6), 0, "GAME OVER");

        // Scale down a bit for other text
        al_identity_transform(&scale_transform);
        al_scale_transform(&scale_transform, 2.0, 2.0);
        al_use_transform(&scale_transform);

        base_width = al_get_text_width(font_, "Press any key to restart");
        scaled_width = base_width * 2;
        al_draw_text(font_, al_map_rgb(WHITE_VEC[0], WHITE_VEC[1], WHITE_VEC[2]),
                     (SCREEN_WIDTH - scaled_width) / 4, (SCREEN_HEIGHT / 4) + UI_TEXT_INFO_HEIGHT * 2, 0, "Press any key to restart");
    }
    else
    {
        int base_width = al_get_text_width(font_, "VICTORY");
        int scaled_width = base_width * 3;

        al_draw_text(font_, al_map_rgb(WHITE_VEC[0], WHITE_VEC[1], WHITE_VEC[2]),
                     (SCREEN_WIDTH - scaled_width) / 6, (SCREEN_HEIGHT / 6), 0, "VICTORY");

        // Scale down a bit for other text
        al_identity_transform(&scale_transform);
        al_scale_transform(&scale_transform, 2.0, 2.0);
        al_use_transform(&scale_transform);

        base_width = al_get_text_width(font_, "Press any key to go next level");
        scaled_width = base_width * 2;
        al_draw_text(font_, al_map_rgb(WHITE_VEC[0], WHITE_VEC[1], WHITE_VEC[2]),
                     (SCREEN_WIDTH - scaled_width) / 4, (SCREEN_HEIGHT / 4) + UI_TEXT_INFO_HEIGHT * 2, 0, "Press any key to go next level");
    }

    // Restore original transform
    al_use_transform(&transform);

    al_flip_display();
}

bool GameScreen::init()
{

    if (!al_init())
    {
        std::cout << "[GAMESCREEN] Error failed to initialize Allegro!" << std::endl;
        return false;
    }

    if (!al_install_keyboard())
    {
        std::cout << "[GAMESCREEN] Error failed to install keyboard!" << std::endl;
        return false;
    }


    if (!al_install_mouse())
    {
        std::cout << "[GAMESCREEN] Error failed to install mouse!" << std::endl;
        return false;
    }

    if (!al_init_primitives_addon())
    {
        std::cout << "[GAMESCREEN] Error failed to initialize primitives addon!" << std::endl;
        return false;
    }

    if (!al_init_font_addon())
    {
        std::cout << "[GAMESCREEN] Error failed to initialize font addon!" << std::endl;
        return false;
    }

    font_ = al_create_builtin_font();
    if (!font_)
    {
        std::cout << "[GAMESCREEN] Error failed to create built-in font!" << std::endl;
        return false;
    }

    display_ = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display_)
    {
        std::cout << "[GAMESCREEN] Error failed to create display!" << std::endl;
        al_destroy_font(font_);
        return false;
    }

    return true;
}