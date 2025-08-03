#pragma once
#include <vector>
#include <string>
#include "../model/Brick.hpp"
#include <fstream>
#include <iostream>

inline int loadScore(const std::string& filename) {
	std::ifstream file(filename);
	
	if (!file.is_open()) {
		std::cout << "[SCORE LOADER] Info: Could not open score file: " << filename << ". Assuming score is 0." << std::endl;
		return 0;  
	}
	
	int score = 0;
	std::string line;
	if (std::getline(file, line)) {
		try {
			score = std::stoi(line);
		} catch (const std::out_of_range& oor) {
			std::cout << "[SCORE LOADER] Error: Score value out of range: " << oor.what() << std::endl;
			return 0;
		}
	}
	
	file.close();
	return score;
}

inline void saveScore(int score, const std::string& filename) {
	std::ofstream file(filename);

	if (!file.is_open()) {
		std::cout << "[SCORE LOADER] Error failed to open score file for writing: " << filename << std::endl;
		return;
	}

	file << score;
	file.close();
}