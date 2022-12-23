#include "../headers/circleParticle.h"

circleParticle::circleParticle(double x, double y, double vx, double vy) : particle(x, y, vx, vy) {
    circleParticle::calcMinMax();
}

circleParticle::circleParticle(double x, double y, double vx, double vy, double mass) : particle(x, y, vx, vy, mass) {
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

void circleParticle::resolveCollision(particle *A, particle *B) {
    double M = B->getMass() * B->getMass() / A->getMass();
    double a = M + B->getMass();
    double b = -1 * (2 * A->getvx() * B->getMass() + 2 * M * B->getvx());
    double c = (B->getMass() - M) * B->getvx() * B->getvx() - 2 * A->getvx() * B->getMass() * B->getvx();
    double tempVB = B->getvx();

    B->setvx((-1*b + sqrt(b*b + 4*a*c))/(2*a));
    A->setvx(A->getvx() + B->getMass()/A->getMass() * (tempVB - B->getvx()));

    b = -1*(2 * A->getvy() * B->getMass() + 2 * M * B->getvy());
    c = (B->getMass() - M) * B->getvy() * B->getvy() - 2 * A->getvy() * B->getMass() * B->getvy();
    tempVB = B->getvy();

    B->setvy((-1*b + sqrt(b*b + 4*a*c))/(2*a));
    A->setvy(A->getvy() + B->getMass()/A->getMass() * (tempVB - B->getvy()));
}