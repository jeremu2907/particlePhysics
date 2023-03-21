//
// Created by Jeremy Nguyen on 1/12/23.
//

#include "../headers/squareParticle.h"

squareParticle::squareParticle(double x, double y, double rotation, double vx, double vy, double va, double mass, double restitution) : particle(x,y,rotation,vx,vy,va,mass,restitution){
    if(mass <= 50)
        this->apothem += mass/20;
    else
        this->apothem = 2.5;
}

void squareParticle::calcMinMax() {
    double deltaX = this->apothem * std::sin(this->getTheta());
    double deltaY = this->apothem * std::cos(this->getTheta());

    deltaX = abs(deltaX);
    deltaY = abs(deltaY);

    this->min[0] = this->x - deltaX - this->apothem;
    this->min[1] = this->y - deltaY - this->apothem;

    this->max[0] = this->x + deltaX + this->apothem;
    this->max[1] = this->y + deltaY + this->apothem;
}

double squareParticle::getShapeCharacteristicValue() {
    return apothem;
}

particle::SHAPE squareParticle::getShape() {
    return particle::SQUARE;
}