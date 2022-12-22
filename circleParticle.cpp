#include "circleParticle.h"

circleParticle::circleParticle(double x, double y, double vx, double vy) : particle::particle(x, y, vx, vy) {
    circleParticle::calcMinMax();
}

void circleParticle::calcMinMax(){
    this->min[0] = this->x - this->radius;
    this->min[1] = this->y - this->radius;

    this->max[0] = this->x + this->radius;
    this->max[1] = this->y + this->radius;
}

int circleParticle::getRadius(){
    return this->radius;
}