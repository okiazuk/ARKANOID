#pragma once
#include "Laser.hpp"
#include <memory>








class Lasers{

public:
	Lasers() = default;

	void createLaser(float x1, float x2=0, float y1=0, float y2=0);
	void removeLaser(const Laser& laser);
	void reset();
	std::vector<std::unique_ptr<Laser>>& getLasers();
	const std::vector<std::unique_ptr<Laser>>& getLasers() const;

private:

	std::vector<std::unique_ptr<Laser>> lasers_;




};