#pragma once
#include "../utils/Configs.hpp"
#include <vector>

enum class BrickColor
{

    WHITE,           // 0
    ORANGE,          // 1
    CYAN,            // 2
    GREEN,           // 3
    RED,             // 4
    BLUE,            // 5
    MAGENTA,         // 6
    YELLOW,          // 7
    SILVER,          // 8
    SILVER_MODIFIED, // 9
    GOLD,            // g
    NONE             // n

};

struct BrickType
{
    BrickColor color;
    std::vector<int> rgb_values;
    int gained_points;
    int width = (BRICK_DEFAULT_WIDTH);
    int height =(BRICK_DEFAULT_HEIGHT);
};

inline const std::vector<int> WHITE_VEC = {255, 255, 255};
inline const std::vector<int> ORANGE_VEC = {255, 165, 0};
inline const std::vector<int> CYAN_VEC = {0, 255, 255};
inline const std::vector<int> GREEN_VEC = {0, 255, 0};
inline const std::vector<int> RED_VEC = {255, 0, 0};
inline const std::vector<int> BLUE_VEC = {0, 0, 255};
inline const std::vector<int> MAGENTA_VEC = {255, 0, 255};
inline const std::vector<int> YELLOW_VEC = {255, 255, 0};
inline const std::vector<int> SILVER_VEC = {192, 192, 192};
inline const std::vector<int> SILVER_MODIFIED_VEC = {140, 140, 150};
inline const std::vector<int> GOLD_VEC = {255, 215, 0};
inline const std::vector<int> BLACKGROUND_VEC = {0, 0, 0};

class Brick
{
public:
    Brick(int color);         // constructor
    ~Brick();                 // destructor
    void hit();               // when a brick is hit by a ball
    bool isDestroyed() const; // check if destroyed
    const BrickType &getBrickType() const;

private:
    const int getPointsFromColor(BrickColor &color) const;
    const std::vector<int> &getRGBFromColor(BrickColor &color) const;
    bool destroyed_ = false;
    BrickType type_;
};