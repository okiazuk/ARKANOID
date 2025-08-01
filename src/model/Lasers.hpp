#pragma once
#include "Laser.hpp"








class Lasers{

public:
	Lasers() = default;
	~Lasers();

	void createLaser(float x1, float x2=0, float y1=0, float y2=0);
	void removeLaser(const Laser& laser);
	void reset();
	std::vector<Laser*>& getLasers();
	const std::vector<Laser*>& getLasers() const;

private:

	std::vector<Laser*> lasers_;




};