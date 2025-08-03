#include "Lasers.hpp"
#include <algorithm>

void Lasers::createLaser(float x1, float x2, float y1, float y2)
{
	std::unique_ptr<Laser> new_laser = std::make_unique<Laser>();
	new_laser->setPositions(x1);
	lasers_.push_back(std::move(new_laser));
}

void Lasers::removeLaser(const Laser &laser)
{

	auto it = std::remove_if(lasers_.begin(), lasers_.end(),
													 [&](const std::unique_ptr<Laser> &b)
													 {
														 return (b.get() == &laser);
													 });
	lasers_.erase(it, lasers_.end());
}

const std::vector<std::unique_ptr<Laser>> &Lasers::getLasers() const
{
	return lasers_;
}

std::vector<std::unique_ptr<Laser>> &Lasers::getLasers()
{
	return lasers_;
}

void Lasers::reset()
{

	lasers_.clear();
	laser_counter_ = DEFAULT_NUMBER_OF_LASER;
}

int Lasers::getRemainingLaser()
{

	return laser_counter_;
}

void Lasers::setRemainingLaser(int number)
{

	laser_counter_ = number;
}