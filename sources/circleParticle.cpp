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

double circleParticle::getShapeCharacteristicValue(){
    return this->radius;
}

void circleParticle::resolveCollisionDirect(circleParticle *A, circleParticle *B) {
    double M = B->getMass() * B->getMass() / A->getMass();
    double a = M + B->getMass();
    double b = (2 * A->getvx() * B->getMass() + 2 * M * B->getvx());
    double c = (B->getMass() - M) * B->getvx() * B->getvx() - 2 * A->getvx() * B->getMass() * B->getvx();
    double tempVB = B->getvx();

    B->setvx((b + sqrt(b*b + 4*a*c))/(2*a));
    A->setvx(A->getvx() + B->getMass()/A->getMass() * (tempVB - B->getvx()));

    b = -1*(2 * A->getvy() * B->getMass() + 2 * M * B->getvy());
    c = (B->getMass() - M) * B->getvy() * B->getvy() - 2 * A->getvy() * B->getMass() * B->getvy();
    tempVB = B->getvy();

    B->setvy((-1*b + sqrt(b*b + 4*a*c))/(2*a));
    A->setvy(A->getvy() + B->getMass()/A->getMass() * (tempVB - B->getvy()));
}

double* circleParticle::intermediateV(double v1, double v2, double m1, double m2) {
    double M = (m2*m2) / m1;
    double a = M + m2;
    double b = (2 * v1 * m2 + 2 * M * v2);
    double c = (m2 - M) * v2 * v2 - 2 * v1 * m2 * v2;
    double v2Prime = (b + sqrt(b*b + 4*a*c))/(2*a);
    return new double[2] {v1 + (m2/m1) * (v2 - v2Prime), v2Prime};
}

void circleParticle::resolveCollision(circleParticle *A, circleParticle *B) {
    //Find Tangent vector
    double tanX = - (B->gety() - A->gety());
    double tanY = B->getx() - A->getx();
    double magTanSq = tanX * tanX + tanY * tanY;

    //Find dot product of component B/A in respect to tan vector
    double ABDotTan = A->getvx() * tanX + A->getvy() * tanY;
    //Parralel vector of AB in the direction of tan vector
    double paraABX = (ABDotTan / (magTanSq)) * tanX;
    double paraABY = (ABDotTan / (magTanSq)) * tanY;

    //Calculating perpendicular vector in respect to tan vector, taking into account mass to conservee momentum
    //Momentum is conserved in its own respective axis (DO NOT CHECK BY DOING PYTHAGOREAN THEOREM BC IT WILL SEEM WRONG
    //Function out put the change in V_axis [vA, vB]
    double * X = intermediateV(A->getvx() - paraABX, B->getvx() - paraABX, A->getMass(), B->getMass());
    double perpAX = (X[0]);
    double perpBX = (X[1]);
    double * Y = intermediateV(A->getvy() - paraABY, B->getvy() - paraABY, A->getMass(), B->getMass());
    double perpAY = (Y[0]);
    double perpBY = (Y[1]);

    A->setvx(paraABX + perpAX);
    A->setvy(paraABY + perpAY);
    B->setvx(paraABX + perpBX);
    B->setvy(paraABY + perpBY);

    delete X;
    delete Y;
}