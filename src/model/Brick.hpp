#pragma once

struct BrickType {
    const int color;
    const int gained_points;
};




class Brick {
public:
    Brick(); // constructor
    ~Brick(); // destructor
    void hit(); // when a brick is hit by a ball
    bool isDestroyed() const; // check if destroyed
    
    int getColor() const; // accessor for color
    int getPoints() const; // accessor for points

private:
    bool destroyed_;
    BrickType type_;
};