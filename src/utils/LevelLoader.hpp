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
        std::cout << "[LEVEL LOARDER] Error Failed to open level file: " << filename << std::endl;
        return level;  
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::vector<Brick> row;

        int length_decoded = 1;
        
        for (char c : line) {
            int brick_color;

            if (c == ' ') {
                continue;
            }

            if (length_decoded == 1){
                // CHECK FIRST CHARACTER
                if (c == 'x'){
                    brick_color = 11;
                } else{
                    brick_color = c - '0';  // '1' becomes 1, '2' becomes 2, etc.
                }
            } else {
                //CHECK SECOND CHARACTER 
                int power = c - '0';

                PowerUps power_up = PowerUps(power);
                row.emplace_back(brick_color, power_up);

                length_decoded = 0;
                }

            length_decoded++;

    }
        
    if (!row.empty()) {
        level.push_back(row);
    }
    }
    
    file.close();
    return level;
}



