#include "Lasers.hpp"
#include <algorithm>


Lasers::~Lasers(){

	for(Laser* laser: lasers_){
		delete laser;
	}
}



void Lasers::createLaser(float x1, float x2, float y1, float y2){
	Laser* new_laser = new Laser();
	new_laser->setPositions(x1);
	lasers_.push_back(new_laser);

}


void Lasers::removeLaser(const Laser& laser){

	    auto it = std::remove_if(lasers_.begin(), lasers_.end(),
        [&](const Laser* b) {
            if (b == &laser) {
                delete b; 
                return true; 
            }
            return false;
        });
    lasers_.erase(it, lasers_.end());


}

const std::vector<Laser*>& Lasers::getLasers() const {
	return lasers_;
}

std::vector<Laser*>& Lasers::getLasers() {
	return lasers_;
}

void Lasers::reset(){

	for (auto& laser: lasers_){
		delete laser;
	}
	lasers_.clear();

}