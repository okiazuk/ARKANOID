#include "model/Brick.hpp"

// Initialize brick with default type and intact state
Brick::Brick()
    : destroyed_(false)
    , type_ { 255, 100 }
{
} // white color, 100 points

Brick::~Brick() { }

void Brick::hit()
{
    destroyed_ = true;
}

bool Brick::isDestroyed() const
{
    return destroyed_;
}

int Brick::getColor() const
{
    return type_.color;
}

int Brick::getPoints() const
{
    return type_.gained_points;
}
