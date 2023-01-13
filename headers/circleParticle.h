#ifndef CIRCLEPARTICLE_H
#define CIRCLEPARTICLE_H

#include "particle.h"
#include "cmath"
#include <iostream>

class circleParticle : public particle {
    private:
        double radius = 1;

        void calcMinMax() override;
    public:
        circleParticle(double x, double y, double vx, double vy);
        circleParticle(double x, double y, double vx, double vy, double mass);
        circleParticle(double x, double y, double vx, double vy, double mass, float restitution);
        ~circleParticle(){};

        double getShapeCharacteristicValue() override;
        particle::SHAPE getShape() override;

        //Used to test head-on direct collision only
        void static resolveCollisionDirect(circleParticle *A, circleParticle *B);
};
#endif