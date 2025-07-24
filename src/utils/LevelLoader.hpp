#pragma once
#include <vector>
#include <string>
#include "../model/Brick.hpp"
#include <fstream>
#include <iostream>


inline std::vector<std::vector<Brick>> loadLevel(const std::string& filename) {
    std::vector<std::vector<Brick>> level;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open level file: " << filename << std::endl;
        return level;  
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::vector<Brick> row;
        
        for (char c : line) {
            int brick_color;
            if (c == 'x'){
                brick_color = 11;
            }else if (c == 'g'){
                brick_color = 10;
            } else{
                brick_color = c - '0';  // '1' becomes 1, '2' becomes 2, etc.
            }
            
            row.emplace_back(brick_color);
        }
        
        if (!row.empty()) {
            level.push_back(row);
        }
    }
    
    file.close();
    return level;
}



