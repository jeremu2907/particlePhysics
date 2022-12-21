#ifndef CIRCLEPARTICLE_H
#define CIRCLEPARTICLE_H

#include "particle.h"

class circleParticle : public particle {
    private:
        int radius = 1;
        void xcollision(){};

    public:
        circleParticle(double x, double y, double vx, double vy) :  particle::particle(x, y, vx, vy){};

        int getRadius();
};
#endif