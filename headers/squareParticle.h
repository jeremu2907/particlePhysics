//
// Created by Jeremy Nguyen on 1/12/23.
//

#ifndef PARTICLEPHYSICS_SQUAREPARTICLE_H
#define PARTICLEPHYSICS_SQUAREPARTICLE_H

#include "particle.h"

class squareParticle : public particle {
    private:
        double apothem = 1;

        void calcMinMax() override;

    public:
        squareParticle(double x, double y, double rotation, double vx, double vy, double va, double mass, double restitution);

        double getShapeCharacteristicValue() override;

        particle::SHAPE getShape() override;
};

#endif //PARTICLEPHYSICS_SQUAREPARTICLE_H
