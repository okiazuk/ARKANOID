#pragma once
#include "../utils/Configs.hpp"
#include <vector>
#include "PowerUps.hpp"

enum class BrickColor
{

    WHITE,           // 0 - 50pts
    ORANGE,          // 1 - 60pts
    CYAN,            // 2 - 70pts
    GREEN,           // 3 - 80pts
    RED,             // 4 - 90pts
    BLUE,            // 5 - 100pts
    MAGENTA,         // 6 - 110pts
    YELLOW,          // 7 - 120pts
    SILVER,          // 8 - 200pts
    GOLD,            // 9
    SILVER_MODIFIED, // brick not used directly because it comes from 8
    NONE             // x

};

struct BrickType
{
    BrickColor color;
    std::vector<int> rgb_values;
    int gained_points;
    int width = (BRICK_DEFAULT_WIDTH);
    int height = (BRICK_DEFAULT_HEIGHT);
};

class Brick
{
public:
    Brick(int color, PowerUps &power_up);
    void hit(float brick_middle_x, float brick_middle_y, bool interruption = false); // when a brick is hit by a ball
    bool isDestroyed() const;
    const BrickType &getBrickType() const;
    const PowerUps &getPowerUp() const; // used in view
    PowerUps &getPowerUp();             // in model

private:
    const int getPointsFromColor(BrickColor &color) const;
    const std::vector<int> &getRGBFromColor(BrickColor &color) const;
    bool destroyed_ = false;
    BrickType type_;
    PowerUps power_up_;
};