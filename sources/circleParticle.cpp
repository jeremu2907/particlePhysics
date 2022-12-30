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
//    std::cout << "Stats_________________________\n";
    double tanX = - (B->gety() - A->gety());
    double tanY = B->getx() - A->getx();
    double magTanSq = tanX * tanX + tanY * tanY;

//    std::cout << "Tangent Vector: " << tanX << '\t' << tanY << std::endl << std::endl;

    //Find dot product of component B/A in respect to tan vector
    double ADotTan = A->getvx() * tanX + A->getvy() * tanY;
    double BDotTan = B->getvx() * tanX + B->getvy() * tanY;
//    std::cout << "A Dot Tan " << ADotTan << "\t B Dot Tan " << BDotTan << std::endl << std::endl;
    //Parralel vector of AB in the direction of tan vector
    double paraAX = (ADotTan / (magTanSq)) * tanX;
    double paraAY = (ADotTan / (magTanSq)) * tanY;
    double paraBX = (BDotTan / (magTanSq)) * tanX;
    double paraBY = (BDotTan / (magTanSq)) * tanY;

//    std::cout << "A Parallel tan: " << paraAX << ", " << paraAY << std::endl;
//    std::cout << "B Parallel tan: " << paraBX << ", " << paraBY << std::endl << std::endl;

    //Calculating perpendicular vector in respect to tan vector, taking into account mass to conservee momentum
    //Momentum is conserved in its own respective axis (DO NOT CHECK BY DOING PYTHAGOREAN THEOREM BC IT WILL SEEM WRONG
    //Function out put the change in V_axis [vA, vB]
//    double * X = intermediateV(A->getvx() - paraBX, B->getvx() - paraABX, A->getMass(), B->getMass());
//    double * X = intermediateV(A->getvx() - paraAX, B->getvx() - paraBX, A->getMass(), B->getMass());
//    double perpAX = (X[0]);
//    double perpBX = (X[1]);
//    double * Y = intermediateV(A->getvy() - paraAY, B->getvy() - paraBY, A->getMass(), B->getMass());
//    double perpAY = (Y[0]);
//    double perpBY = (Y[1]);
    double perpAX = A->getvx() - paraAX;
    double perpAY = A->getvy() - paraAY;
    double perpBX = B->getvx() - paraBX;
    double perpBY = B->getvy() - paraBY;
//    double* X = circleParticle::intermediateV(perpAX, perpBX, A->getMass(), B->getMass());
//    double* Y = circleParticle::intermediateV(perpAY, perpBY, A->getMass(), B->getMass());

//    std::cout << "A Perp tan: " << perpAX << ", " << perpAY << std::endl;
//    std::cout << "B Perp tan: " << perpBX << ", " << perpBY << std::endl;
//
//    std::cout << X[0] << '\t' << Y[0] << std::endl;
//    std::cout << X[1] << '\t' << Y[1] << std::endl;

    A->setvx(paraAX + perpBX);
    A->setvy(paraAY + perpBY);
    B->setvx(paraBX + perpAX);
    B->setvy(paraBY + perpAY);

//    A->setvx(paraAX + Y[1]);
//    A->setvy(paraAY + X[0]);
//    B->setvx(paraBX + X[1]);
//    B->setvy(paraBY + Y[0]);

//    delete X;
//    delete Y;
}