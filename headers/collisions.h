//
// Created by Jeremy Nguyen on 12/22/22.
// Class holds a list of particles to check for collision
// Collision splits into 2 phases: Broad phase -> Narrow phase
//

#ifndef PARTICLEPHYSICS_COLLISIONS_H
#define PARTICLEPHYSICS_COLLISIONS_H

#include <vector>
#include <algorithm>
#include "particle.h"

class collisions{
    private:
        std::vector<particle*> particleListX;
        std::vector<particle*> particleListY;

        double varianceX;
        double varianceY;

        void insertionSort(std::vector<particle*> &);

    public:
        collisions(){};
        collisions(particle*p[]);
        ~collisions(){};

        std::vector<particle*>getParticleList(){
            return (varianceX >= varianceY)? particleListX : particleListY;
        }

        //Adding new particle to list of particles
        void addParticle(particle* ptrParticle);

        //Resort list
        void updateParticle();
        void sweep();
};

#endif //PARTICLEPHYSICS_COLLISIONS_H
