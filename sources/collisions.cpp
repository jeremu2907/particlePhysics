//
// Created by Jeremy Nguyen on 12/22/22.
//

#include "../headers/collisions.h"

collisions::collisions(particle* e[]){
    for(int i = 0; i < sizeof(*e) / sizeof(e[0]); i++){
        particleList.push_back(e[i]);
    }
    std::sort(particleList.begin(), particleList.end(), [](particle* a, particle* b){
        return (a->getMin()[0] < b->getMin()[0])? true : false;
    });
}

void collisions::addParticle(particle *ptrParticle) {
    particleList.push_back(ptrParticle);
    collisions::updateParticle();
}

void collisions::updateParticle() {
    collisions::insertionSort();
}

void collisions::insertionSort() {
    for(int i = 0; i < particleList.size(); i++){
        if(i > 0 && particleList[i]->getx() < particleList[i - 1]->getx()){
            particle* temp = particleList.at(i);
            int j = i - 1;
            while(particleList.at(j)->getx() > temp->getx() && j >= 0){
                particleList.at(j + 1) = particleList.at(j);
                --j;
            }
            particleList.at(j + 1) = temp;
        }
    }
}

void collisions::sweep(){

}