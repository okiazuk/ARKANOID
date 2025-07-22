#include "view/GameScreen.hpp"


GameScreen::GameScreen() {}

void GameScreen::draw(const Board &board, const Ball &ball, const Racket &racket, const GameStats &stats)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Draw bricks
    const auto &bricks = board.getBricks();
    int rows = board.getHeight();
    int cols = board.getWidth();
    int brickWidth = 40;
    int brickHeight = 20;
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            const Brick &brick = bricks[r][c];
            if (!brick.isDestroyed())
            {
                al_draw_filled_rectangle(
                    c * brickWidth, r * brickHeight,
                    c * brickWidth + brickWidth, r * brickHeight + brickHeight,
                    al_map_rgb(brick.getColor(), 255, 255));
            }
        }
    }

    // Draw racket
    RacketPositions racket_pos = racket.getPositions();
    RacketParameters racket_param = racket.getParameters();

    al_draw_filled_rectangle(
        racket_pos.x, racket_pos.y,
        racket_pos.x + racket_param.width, racket_pos.y + racket_param.height,
        al_map_rgb(255, 255, 255));

    // Draw ball
    BallPositions ball_pos = ball.getPositions();
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
    if (!al_init_font_addon()) {
        std::cerr << "Failed to initialize font addon!" << std::endl;
        return false;
    }

    // Use built-in font instead of loading TTF
    font_ = al_create_builtin_font();
    if (!font_) {
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

void GameScreen::drawUI(const GameStats& stats) {

    ALLEGRO_TRANSFORM transform;
    al_copy_transform(&transform, al_get_current_transform());
    
    // Scale text by 2x
    ALLEGRO_TRANSFORM scale_transform;
    al_identity_transform(&scale_transform);
    al_scale_transform(&scale_transform, 2.0, 2.0);
    al_use_transform(&scale_transform);

    // Draw a horizontal line to separate game area from UI
    al_draw_line(0, UI_TEXT_AREA*3, SCREEN_WIDTH/2, UI_TEXT_AREA*3, 
                 al_map_rgb(255, 255, 255), 2.0);

    // Print score left side
    char score_text[50];
    sprintf(score_text, "Score: %d", stats.getBasicInfos().score);
    al_draw_text(font_, al_map_rgb(255, 255, 255), UI_TEXT_AREA, UI_TEXT_AREA, 
                0, score_text);
    
    // Print score right side
    char lives_text[50];
    sprintf(lives_text, "Lives: %d", stats.getBasicInfos().lives);
    al_draw_text(font_, al_map_rgb(255, 255, 255), (SCREEN_WIDTH - 150)/2,
                 UI_TEXT_AREA, 0, lives_text);

    // Restore original transform
    al_use_transform(&transform);

}