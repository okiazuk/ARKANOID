#pragma once



//======================GAME CONSTANTS===========================

//----DISPLAY---- 

/**
 * @brief using 16:9 ratio for compatibility with modern screen 
 * and not too much resolution,performance wise
 */

const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;

struct SCREEN_MIDDLE_POSITION {
  static constexpr int WIDTH = SCREEN_WIDTH / 2;
  static constexpr int HEIGHT = SCREEN_HEIGHT / 2;
};


//---DEFAULT ELEMENTS SIZE---

//--racket--

const int RACKET_WIDTH = 80;
const int RACKET_HEIGHT = 20;
const int RACKET_SPEED = 5;
const int RACKET_BOTTOM_POSITION = 800;


//--ball--

const int BALL_BOTTOM_POSITION = 790;
const int BALL_RADIUS = 10;
const int BALL_DEFAULT_DX = 0;
const int BALL_DEFAULT_DY = -10;