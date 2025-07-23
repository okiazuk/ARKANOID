#pragma once
#include "../utils/Configs.hpp"
#include <vector>

enum class BrickColor{

    NONE, // 0
    WHITE, // 1
    ORANGE, // 2
    CYAN, // 3
    GREEN, // 4
    RED, // 5
    BLUE, // 6
    MAGENTA, // 7
    YELLOW, // 8
    SILVER, // 9
    SILVER_MODIFIED,
    GOLD, // 10
};


struct BrickType {
    BrickColor color;
    std::vector<int> rgb_values;
    int gained_points;
    int width = BRICK_DEFAULT_WIDTH;
    int height = BRICK_DEFAULT_HEIGHT;
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
inline const std::vector<int> BLACKGROUND_VEC = {0, 0, 0};


class Brick {
public:
    Brick(int color); // constructor
    ~Brick(); // destructor
    void hit(); // when a brick is hit by a ball
    bool isDestroyed() const; // check if destroyed
    const BrickType& getBrickType() const;


private:
    const int getPointsFromColor(BrickColor& color) const;
    const std::vector<int>& getRGBFromColor(BrickColor& color) const;
    bool destroyed_ = false;
    BrickType type_;
};