#include "view/GameScreen.hpp"
#include "../utils/Configs.hpp"

/**
 * @brief everything should const because we simply print from the model
 * model is using float and view is casting in int
 */

GameScreen::GameScreen() {}

void GameScreen::drawGame(const Board &board, const Ball &ball, const Racket &racket, const GameStats &stats)
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
                al_draw_rectangle(x1, y1, x2, y2, al_map_rgb(BLACKGROUND_VEC[0], BLACKGROUND_VEC[1], BLACKGROUND_VEC[2]), 1.0);
            }
        }
    }

    // Draw racket
    const RacketPositions &racket_pos = racket.getPositions();
    const RacketParameters &racket_param = racket.getParameters();

    al_draw_filled_rectangle(
        racket_pos.x, racket_pos.y,
        racket_pos.x + racket_param.width, racket_pos.y + racket_param.height,
        al_map_rgb(WHITE_VEC[0], WHITE_VEC[1], WHITE_VEC[2]));

    // Draw ball with proper rounding
    const BallPositions &ball_pos = ball.getPositions();
    al_draw_filled_circle(
        static_cast<int>(ball_pos.x + 0.5f),     // explicit conversion with rounding
        static_cast<int>(ball_pos.y + 0.5f),     
        static_cast<int>(ball.getParameters().radius + 0.5f),  
        al_map_rgb(WHITE_VEC[0], WHITE_VEC[1], WHITE_VEC[2]));

    drawUI(stats);

    al_flip_display();
}

void GameScreen::destroy()
{
    al_destroy_display(display_);
}

void GameScreen::drawUI(const GameStats &stats)
{


    // Draw lines
    al_draw_line(0, SEPARATION_LINE_HEIGHT, SCREEN_WIDTH,  SEPARATION_LINE_HEIGHT,
        al_map_rgb(WHITE_VEC[0], WHITE_VEC[0], WHITE_VEC[0]), 1.0);

    al_draw_line(0, 0, SCREEN_WIDTH,  0,
        al_map_rgb(WHITE_VEC[0], WHITE_VEC[0], WHITE_VEC[0]), 2.0);
    
    al_draw_line(0, 0, 0,  SCREEN_HEIGHT,
        al_map_rgb(WHITE_VEC[0], WHITE_VEC[0], WHITE_VEC[0]), 2.0);

    al_draw_line(SCREEN_WIDTH, 0, SCREEN_WIDTH,  SCREEN_HEIGHT,
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
    al_draw_text(font_, al_map_rgb(WHITE_VEC[0], WHITE_VEC[0], WHITE_VEC[0]), UI_TEXT_HEIGHT, UI_TEXT_HEIGHT,
                 0, score_text);

    // Print score right side
    char lives_text[50];
    sprintf(lives_text, "Lives: %d", stats.getBasicInfos().lives);
    al_draw_text(font_, al_map_rgb(WHITE_VEC[0], WHITE_VEC[0], WHITE_VEC[0]), UI_TEXT_WIDTH_LIVES,
                 UI_TEXT_HEIGHT, 0, lives_text);

    // Restore original transform
    al_use_transform(&transform);
}


void GameScreen::drawEndGame(){

    std::cout << "[GAME SCREEN] END GAME SCREEN" << std::endl;

}



bool GameScreen::init()
{

    // Initialize Allegro
    if (!al_init())
    {
        std::cerr << "Failed to initialize Allegro!" << std::endl;
        return false;
    }

    // Install keyboard
    if (!al_install_keyboard())
    {
        std::cerr << "Failed to install keyboard!" << std::endl;
        return false;
    }

    // Install mouse

    if (!al_install_mouse())
        {
            std::cerr << "Failed to install mouse!" << std::endl;
            return false;
        }

    // Initialize primitives addon
    if (!al_init_primitives_addon())
    {
        std::cerr << "Failed to initialize primitives addon!" << std::endl;
        return false;
    }

    // Initialize font addon (still needed for built-in font)
    if (!al_init_font_addon())
    {
        std::cerr << "Failed to initialize font addon!" << std::endl;
        return false;
    }

    // Use built-in font instead of loading TTF
    font_ = al_create_builtin_font();
    if (!font_)
    {
        std::cerr << "Failed to create built-in font!" << std::endl;
        return false;
    }

    // Create display
    display_ = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display_)
    {
        std::cerr << "Failed to create display!" << std::endl;
        al_destroy_font(font_);
        return false;
    }

    return true;
}