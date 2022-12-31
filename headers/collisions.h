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
#include "circleParticle.h"

class collisions{
    private:
        std::vector<particle*> particleListX;
        std::vector<particle*> particleListY;
        float coefOfRestitution = 1;    //[0,1] perfect inelastic - perfect elastic

        double varianceX;
        double varianceY;

        void insertionSort(std::vector<particle*> &, int axis);
        bool testCollision(particle* A, particle *B);

        int getParticleList(){
            return (varianceX >= varianceY)? 0 : 1;
        }

    public:
        double totalCalculations = 0;

        collisions(){};
        collisions(std::vector<particle*>);
        ~collisions(){};

        std::vector<particle*> getList(){
            return (getParticleList())? particleListY : particleListX;
        }

        //Adding new particle to list of particles
        void addParticle(particle* ptrParticle);

        //Resort list
        void updateParticle();

        //Check for Collision
        void checkForCollision();
};

#endif //PARTICLEPHYSICS_COLLISIONS_H
