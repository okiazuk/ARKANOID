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
    SILVER_MODIFIED,  // brick not used directly because it comes from 8
    GOLD,            // g
    NONE             // x

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
inline const std::vector<int> PURPLE_UP_VEC = {147, 112, 219}; // Bright purple for power-ups

class Brick
{
public:
    Brick(int color, PowerUps& power_up);      // constructor
    ~Brick();                 // destructor
    void hit(float brick_middle_x, float brick_middle_y);   // when a brick is hit by a ball
    bool isDestroyed() const; // check if destroyed
    const BrickType &getBrickType() const;
    const PowerUps& getPowerUp() const;  
    PowerUps& getPowerUp();              
    

private:
    const int getPointsFromColor(BrickColor &color) const;
    const std::vector<int> &getRGBFromColor(BrickColor &color) const;
    bool destroyed_ = false;
    BrickType type_;
    PowerUps power_up_;

};