#ifndef CIRCLEPARTICLE_H
#define CIRCLEPARTICLE_H

#include "particle.h"
#include "cmath"
#include <iostream>

class circleParticle : public particle {
    private:
        int radius = 1;

        void calcMinMax() override;

    public:
        circleParticle(double x, double y, double vx, double vy);
        circleParticle(double x, double y, double vx, double vy, double mass);
        ~circleParticle(){};

        int getRadius();
        void static resolveCollision(particle *A, particle *B);
};
#endif