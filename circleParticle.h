#ifndef CIRCLEPARTICLE_H
#define CIRCLEPARTICLE_H

#include "particle.h"
#include "cmath"

class circleParticle : public particle {
    private:
        int radius = 1;

        void calcMinMax() override;

    public:
        circleParticle(double x, double y, double vx, double vy);
        ~circleParticle(){};

        int getRadius();
};
#endif