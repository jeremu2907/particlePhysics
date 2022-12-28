//
// Created by Jeremy Nguyen on 12/22/22.
//

#include "../headers/collisions.h"

collisions::collisions(std::vector<particle*> e){
    for(int i = 0; i < e.size(); i++){
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

bool collisions::testCollision(particle *A, particle *B) {
    double sqDist = (B->getx() - A->getx()) * (B->getx() - A->getx()) + (B->gety() - A->gety()) * (B->gety() - A->gety());
    double sqDiffCenter = (A->getShapeCharacteristicValue() + B->getShapeCharacteristicValue());
    sqDiffCenter *= sqDiffCenter;

//    std::printf("dqdist: %f       sqDiff: %f\n", sqDist, sqDiffCenter);

    if(sqDist <= sqDiffCenter)
        return true;
    else
        return false;
}

void collisions::checkForCollision(){
   if (collisions::particleListX.size() == 1)
       return;
   int axis = collisions::getParticleList();
   std::vector<particle*> List = (axis)? particleListX : particleListY;

   int  queueFront = 0,
        queueBack  = 0;

   while(queueFront < List.size()){
       if(List[queueBack]->getMin()[axis] <= List[queueFront]->getMax()[axis]
            && queueBack < List.size() - 1){     //Possible collision
            queueBack++;
       }        //No possible collision
       else{
           if(queueBack - queueFront != 0){     //More than one particle in queue
               while(queueFront < queueBack) {
                   for (int j = queueFront + 1; j <= queueBack; j++) {
//                       if(List[j]->getMin()[axis] > List[queueFront]->getMax()[axis])
//                           break;
                       totalCalculations++;
                       if (testCollision(List[queueFront], List[j])) {
                           circleParticle::resolveCollision(dynamic_cast<circleParticle *> (List[queueFront]),
                                                            dynamic_cast<circleParticle *> (List[j]));
                       }
                   }
                   queueFront++;
               }
           } else {
               queueFront++;
           }
       }
   }
}