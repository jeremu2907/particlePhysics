#ifndef CIRCLEPARTICLE_H
#define CIRCLEPARTICLE_H

#include "particle.h"
#include "cmath"
#include <iostream>

class circleParticle : public particle {
    private:
        int radius = 1;

        void calcMinMax() override;
        static double* intermediateV(double, double, double, double); //Helper function for circleParticle::resolveCollision
    public:
        circleParticle(double x, double y, double vx, double vy);
        circleParticle(double x, double y, double vx, double vy, double mass);
        ~circleParticle(){};

        int getRadius();
        void static resolveCollision(circleParticle *A, circleParticle *B);


        //Used to test head-on direct collision only
        void static resolveCollisionDirect(circleParticle *A, circleParticle *B);
};
#endif