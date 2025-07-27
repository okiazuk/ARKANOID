cmake_minimum_required(VERSION 3.20)

project(ARKANOID)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use pkg-config to find Allegro5
find_package(PkgConfig REQUIRED)
pkg_check_modules(ALLEGRO REQUIRED 
    allegro-5 
    allegro_main-5 
    allegro_primitives-5 
    allegro_font-5 
    allegro_ttf-5 
    allegro_image-5
)

# Add all source files to the executable
add_executable(ARKANOID
    src/main.cpp
    src/model/Ball.cpp
    src/model/Board.cpp
    src/model/Brick.cpp
    src/model/Racket.cpp
    src/model/GameStats.cpp
    src/controller/GameControl.cpp
    src/view/GameScreen.cpp
    src/utils/GameStates.cpp
    src/model/PowerUps.cpp
)

target_include_directories(ARKANOID PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${ALLEGRO_INCLUDE_DIRS}
)

target_link_libraries(ARKANOID
    PRIVATE
    ${ALLEGRO_LIBRARIES}
)
