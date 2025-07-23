#include "model/Brick.hpp"

// Initialize brick with default type and intact state
Brick::Brick(int color)
    : destroyed_(false)
{
type_.color = BrickColor(color);
type_.gained_points = getPointsFromColor(type_.color);
type_.rgb_values = getRGBFromColor(type_.color);
} 

Brick::~Brick() { }

void Brick::hit()
{
    if (type_.color != BrickColor::SILVER || type_.color != BrickColor::GOLD){
        destroyed_ = true;
    } else if (type_.color == BrickColor::SILVER){
        type_.color = BrickColor::SILVER_MODIFIED;
        }
}

bool Brick::isDestroyed() const
{
    return destroyed_;
}


const BrickType& Brick::getBrickType() const {
    return type_;
}


const int Brick::getPointsFromColor(BrickColor& color) const {  
    switch (color) {
        case BrickColor::WHITE:
            return WHITE_POINTS;
        case BrickColor::ORANGE:
            return ORANGE_POINTS;
        case BrickColor::CYAN:
            return CYAN_POINTS;
        case BrickColor::GREEN:
            return GREEN_POINTS;
        case BrickColor::RED:
            return RED_POINTS;
        case BrickColor::BLUE:
            return BLUE_POINTS;
        case BrickColor::MAGENTA:
            return MAGENTA_POINTS;
        case BrickColor::YELLOW:
            return YELLOW_POINTS;
        case BrickColor::SILVER:
            return SILVER_POINTS;
        default:
            return 0;  
    }
}


const std::vector<int>& Brick::getRGBFromColor(BrickColor& color) const {
    
    switch (color) {
        case BrickColor::WHITE:
            return WHITE_VEC;
        case BrickColor::ORANGE:
            return ORANGE_VEC;
        case BrickColor::CYAN:
            return CYAN_VEC;
        case BrickColor::GREEN:
            return GREEN_VEC;
        case BrickColor::RED:
            return RED_VEC;
        case BrickColor::BLUE:
            return BLUE_VEC;
        case BrickColor::MAGENTA:
            return MAGENTA_VEC;
        case BrickColor::YELLOW:
            return YELLOW_VEC;
        case BrickColor::SILVER:
            return SILVER_VEC;
        case BrickColor::NONE:
             return BLACK_VEC;
        default: 
            return BLACK_VEC;
    }
}