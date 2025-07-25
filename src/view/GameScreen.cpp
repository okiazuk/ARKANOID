#include "view/GameScreen.hpp"
#include "../utils/Configs.hpp"

/**
 * @brief everything should const because we simply print from the model
 * model is using int and view is casting in int
 */


void GameScreen::draw(Board& board, Ball& ball, Racket& racket, GameStats& stats){


    if (CURRENT_GAME_STATE == GameStates::IN_GAME){

        drawInGame(board, ball, racket, stats);

    } else if (CURRENT_GAME_STATE == GameStates::END_GAME){
        drawEndGame(stats);
    } else if (CURRENT_GAME_STATE == GameStates::MAIN_MENU){
        drawMainMenu();
    }



}

void GameScreen::drawInGame(const Board &board, const Ball &ball, const Racket &racket, const GameStats &stats)
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
            }
        }
    }

    // Draw racket
    const RacketPositions &racket_pos = racket.getPositions();
    const RacketParameters &racket_param = racket.getParameters();

    al_draw_filled_rectangle(
        (racket_pos.x + +0.5f),(racket_pos.y + +0.5f),
        (racket_pos.x + +0.5f) + racket_param.width, (racket_pos.y + +0.5f) + racket_param.height,
        al_map_rgb(WHITE_VEC[0], WHITE_VEC[1], WHITE_VEC[2]));

    // Draw ball with proper rounding
    const BallPositions &ball_pos = ball.getPositions();
    al_draw_filled_circle(
        static_cast<int>(ball_pos.x + 0.5f), // explicit conversion with rounding
        static_cast<int>(ball_pos.y + 0.5f),
        static_cast<int>(ball.getParameters().radius + 0.5f),
        al_map_rgb(WHITE_VEC[0], WHITE_VEC[1], WHITE_VEC[2]));

    drawUI(stats, board);

    al_flip_display();
}

void GameScreen::destroy()
{
    al_destroy_display(display_);
}

void GameScreen::drawUI(const GameStats &stats, const Board& board)
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

    
    // Print Best Score
    char best_score_text[50];
    sprintf(best_score_text, "Best Score: %d", loadScore(SCORE_PATH));
    al_draw_text(font_, al_map_rgb(WHITE_VEC[0], WHITE_VEC[0], WHITE_VEC[0]), UI_TEXT_INFO_WIDTH_BETWEEN_WALLS*7, UI_TEXT_INFO_HEIGHT,
                 0, best_score_text);

    

    // Print Level 
    char level[50];
    sprintf(level, "Level: %d", board.getLevelNumber());
    al_draw_text(font_, al_map_rgb(WHITE_VEC[0], WHITE_VEC[0], WHITE_VEC[0]), UI_TEXT_INFO_WIDTH_BETWEEN_WALLS*17, UI_TEXT_INFO_HEIGHT,
                 0, level);



    const int base_width = al_get_text_width(font_, "Lives:  ");
    const int scaled_width = base_width * 2.0;

    // Print score right side
    char lives_text[50];
    sprintf(lives_text, "Lives: %d", stats.getBasicInfos().lives);
    al_draw_text(font_, al_map_rgb(WHITE_VEC[0], WHITE_VEC[0], WHITE_VEC[0]), (SCREEN_WIDTH - scaled_width)/2 - UI_TEXT_INFO_WIDTH_BETWEEN_WALLS,
                 UI_TEXT_INFO_HEIGHT, 0, lives_text);

    // Restore original transform
    al_use_transform(&transform);
}



void GameScreen::drawMainMenu(){


}



void GameScreen::drawEndGame(const GameStats& stats)
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


    if (game_over){
        int base_width = al_get_text_width(font_, "GAME OVER");
        int scaled_width = base_width * 3;

        al_draw_text(font_, al_map_rgb(WHITE_VEC[0], WHITE_VEC[1], WHITE_VEC[2]), 
                (SCREEN_WIDTH - scaled_width) /6, (SCREEN_HEIGHT/6), 0, "GAME OVER");

    }else {
        int base_width = al_get_text_width(font_, "VICTORY");
        int scaled_width = base_width * 3;

        al_draw_text(font_, al_map_rgb(WHITE_VEC[0], WHITE_VEC[1], WHITE_VEC[2]), 
        (SCREEN_WIDTH - scaled_width) /6, (SCREEN_HEIGHT/6), 0, "VICTORY");
    }

    // Scale down a bit for other text
    al_identity_transform(&scale_transform);
    al_scale_transform(&scale_transform, 2.0, 2.0);
    al_use_transform(&scale_transform);


    int base_width = al_get_text_width(font_, "Press any key to restart");
    int scaled_width = base_width * 2;
    
    // Add restart instruction
    al_draw_text(font_, al_map_rgb(WHITE_VEC[0], WHITE_VEC[1], WHITE_VEC[2]), 
                 (SCREEN_WIDTH - scaled_width) / 4, (SCREEN_HEIGHT/4) + UI_TEXT_INFO_HEIGHT*2, 0, "Press any key to restart");

    // Restore original transform
    al_use_transform(&transform);

    al_flip_display();
}




bool GameScreen::init()
{

    // Initialize Allegro
    if (!al_init())
    {
        std::cout << "[GAMESCREEN] Error failed to initialize Allegro!" << std::endl;
        return false;
    }

    // Install keyboard
    if (!al_install_keyboard())
    {
        std::cout << "[GAMESCREEN] Error failed to install keyboard!" << std::endl;
        return false;
    }

    // Install mouse

    if (!al_install_mouse())
    {
        std::cout << "[GAMESCREEN] Error failed to install mouse!" << std::endl;
        return false;
    }

    // Initialize primitives addon
    if (!al_init_primitives_addon())
    {
        std::cout << "[GAMESCREEN] Error failed to initialize primitives addon!" << std::endl;
        return false;
    }

    // Initialize font addon (still needed for built-in font)
    if (!al_init_font_addon())
    {
        std::cout << "[GAMESCREEN] Error failed to initialize font addon!" << std::endl;
        return false;
    }

    // Use built-in font instead of loading TTF
    font_ = al_create_builtin_font();
    if (!font_)
    {
        std::cout << "[GAMESCREEN] Error failed to create built-in font!" << std::endl;
        return false;
    }

    // Create display
    display_ = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display_)
    {
        std::cout << "[GAMESCREEN] Error failed to create display!" << std::endl;
        al_destroy_font(font_);
        return false;
    }

    return true;
}