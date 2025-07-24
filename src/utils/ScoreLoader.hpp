#pragma once
#include <vector>
#include <string>
#include "../model/Brick.hpp"
#include <fstream>
#include <iostream>

inline const int loadScore(const std::string& filename) {
    std::vector<std::vector<Brick>> level;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open level file: " << filename << std::endl;
        return 0;  
    }
    
    std::string line;
    while (std::getline(file, line)) {
			try {
				return std::stoi(line);
			} catch (const std::exception& e) {
				std::cerr << "Failed to convert score: " << e.what() << std::endl;
				return 0;
			}
			
    }
		return 0;
    
    file.close();
}

inline void saveScore(int score, const std::string& filename) {
	std::ofstream file(filename);

	if (!file.is_open()) {
		std::cerr << "Failed to open score file for writing: " << filename << std::endl;
		return;
	}

	file << score;
	file.close();
}