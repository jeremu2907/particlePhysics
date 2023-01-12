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

    this->min[0] = this->x - deltaX;
    this->min[1] = this->y - deltaY;

    this->max[0] = this->x + deltaX;
    this->max[1] = this->y + deltaY;
}

double squareParticle::getShapeCharacteristicValue() {
    return apothem;
}

//particle::SHAPE getShape() {
//    return particle::SHAPE::SQUARE;
//}