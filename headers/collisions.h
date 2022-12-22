//
// Created by Jeremy Nguyen on 12/22/22.
// Class holds a list of particles to check for collision
// Collision splits into 2 phases: Broad phase -> Narrow phase
//

#ifndef PARTICLEPHYSICS_COLLISIONS_H
#define PARTICLEPHYSICS_COLLISIONS_H

#include <vector>
#include "particle.h"

class collisions{
    private:
        std::vector<particle*> particleList;

        void resolveCollision(std::vector<particle> * ptrParticlelist){};

    public:
        collisions(){};
        ~collisions(){};

        std::vector<particle*>getParticleList(){
            return particleList;
        }

        void addParticle(particle* ptrParticle);
        void updateParticle();
        void sweep();
};

#endif //PARTICLEPHYSICS_COLLISIONS_H
