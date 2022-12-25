//
// Created by Jeremy Nguyen on 12/22/22.
//

#include "../headers/collisions.h"

collisions::collisions(particle* e[]){
    for(int i = 0; i < sizeof(*e) / sizeof(e[0]); i++){
        particleListX.push_back(e[i]);
        particleListY.push_back(e[i]);
    }
    std::sort(particleListX.begin(), particleListX.end(), [](particle* a, particle* b){
        return (a->getMin()[0] < b->getMin()[0])? true : false;
    });

    std::sort(particleListY.begin(), particleListY.end(), [](particle* a, particle* b){
        return (a->getMin()[1] < b->getMin()[1])? true : false;
    });
}

void collisions::addParticle(particle *ptrParticle) {
    particleListX.push_back(ptrParticle);
    particleListY.push_back(ptrParticle);
    collisions::updateParticle();
}

void collisions::updateParticle() {
    collisions::insertionSort(particleListX);
    collisions::insertionSort(particleListY);

    double meanX = 0;
    double meanY = 0;
    for(int i = 0 ; i < particleListX.size(); i++){
        meanX += particleListX[i]->getMin()[0];
        meanY += particleListY[i]->getMin()[1];
    }
    meanX /= particleListX.size();
    meanY /= particleListY.size();

    collisions::varianceX = 0;
    collisions::varianceY = 0;
    for(int i = 0 ; i < particleListX.size(); i++){
        varianceX += (particleListX[i]->getMin()[0] - meanX) * (particleListX[i]->getMin()[0] - meanX);
        varianceY += (particleListY[i]->getMin()[1] - meanY) * (particleListY[i]->getMin()[1] - meanY);
    }

    collisions::varianceX /= (particleListX.size() - 1);
    collisions::varianceY /= (particleListY.size() - 1);
}

void collisions::insertionSort(std::vector<particle*> &list) {
    for(int i = 0; i < list.size(); i++){
        if(i > 0 && list[i]->getx() < list[i - 1]->getx()){
            particle* temp = list.at(i);
            int j = i - 1;
            while(list.at(j)->getx() > temp->getx() && j >= 0){
                list.at(j + 1) = list.at(j);
                --j;
            }
            list.at(j + 1) = temp;
        }
    }
}

void collisions::sweep(){

}