//
// Created by Jeremy Nguyen on 12/22/22.
//

#include "../headers/collisions.h"

void collisions::addParticle(particle *ptrParticle) {
    particleList.push_back(ptrParticle);
    collisions::updateParticle();
}

void collisions::updateParticle() {
    std::sort(particleList.begin(), particleList.end(), [](particle* a, particle* b){
        return (a->getMin()[0] < b->getMin()[0])? true : false;
    });
}

void collisions::sweep(){

}