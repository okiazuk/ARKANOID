#include "view/GameScreen.hpp"

GameScreen::GameScreen() { }

void GameScreen::draw(const Board& board, const Ball& ball, const Racket& racket, const GameStats& stats)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Draw bricks
    const auto& bricks = board.getBricks();
    int rows = board.getHeight();
    int cols = board.getWidth();
    int brickWidth = 40;
    int brickHeight = 20;
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            const Brick& brick = bricks[r][c];
            if (!brick.isDestroyed()) {
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

    std::cout << racket_pos.x << std::endl;
    std::cout << racket_pos.y << std::endl;
    
    al_draw_filled_rectangle(
        racket_pos.x, racket_pos.y,
        racket_pos.x + racket_param.width, racket_pos.y + racket_param.height,
        al_map_rgb(255, 255, 255));

    // Draw ball
    BallPositions ball_pos = ball.getPositions();
    al_draw_filled_circle(
        ball_pos.x, ball_pos.y, ball.getRadius(),
        al_map_rgb(255, 255, 255));

    al_flip_display();
}

void GameScreen::destroy()
{
    al_destroy_display(display_);
}

bool GameScreen::init()
{

    // Initialize Allegro
    if (!al_init()) {
        std::cerr << "Failed to initialize Allegro!" << std::endl;
        return false;
    }

    // Install keyboard
    if (!al_install_keyboard()) {
        std::cerr << "Failed to install keyboard!" << std::endl;
        return false;
    }

    // Initialize primitives addon
    if (!al_init_primitives_addon()) {
        std::cerr << "Failed to initialize primitives addon!" << std::endl;
        return false;
    }
    // Create display
    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!display) {
        std::cerr << "Failed to create display!" << std::endl;
        return false;
    }

    return true;
}