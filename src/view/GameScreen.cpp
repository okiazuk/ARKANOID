#include "view/GameScreen.hpp"
#include "../utils/Configs.hpp"

GameScreen::GameScreen() {}

void GameScreen::draw(const Board &board, const Ball &ball, const Racket &racket, const GameStats &stats)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

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

            if (!brick.isDestroyed())
            {
                al_draw_filled_rectangle(
                    c * brick_width, r * brick_height,
                    c * brick_width + brick_width, r * brick_height + brick_height,
                    al_map_rgb(brick_rgb[0], brick_rgb[1], brick_rgb[2]));
            }
        }
    }

    // Draw racket
    const RacketPositions &racket_pos = racket.getPositions();
    const RacketParameters &racket_param = racket.getParameters();

    al_draw_filled_rectangle(
        racket_pos.x, racket_pos.y,
        racket_pos.x + racket_param.width, racket_pos.y + racket_param.height,
        al_map_rgb(255, 255, 255));

    // Draw ball
    const BallPositions &ball_pos = ball.getPositions();
    const int ball_radius = ball.getParameters().radius;
    al_draw_filled_circle(
        ball_pos.x, ball_pos.y, ball_radius,
        al_map_rgb(255, 255, 255));

    drawUI(stats);

    al_flip_display();
}

void GameScreen::destroy()
{
    al_destroy_display(display_);
}

void GameScreen::drawUI(const GameStats &stats)
{

    ALLEGRO_TRANSFORM transform;
    al_copy_transform(&transform, al_get_current_transform());

    // Scale text by 2x
    ALLEGRO_TRANSFORM scale_transform;
    al_identity_transform(&scale_transform);
    al_scale_transform(&scale_transform, 2.0, 2.0);
    al_use_transform(&scale_transform);

    // Draw a horizontal line to separate game area from UI
    al_draw_line(0, SEPARATION_LINE_HEIGHT, SCREEN_WIDTH,  SEPARATION_LINE_HEIGHT,
                 al_map_rgb(WHITE_VEC[0], WHITE_VEC[0], WHITE_VEC[0]), 2.0);

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