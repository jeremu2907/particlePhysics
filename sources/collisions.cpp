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

    collisions::insertionSort(particleListX, 0);
    collisions::insertionSort(particleListY, 1);
}

void collisions::insertionSort(std::vector<particle*> &list, int axis) {
    for(int i = 0; i < list.size(); i++){
        if(axis == 0) {
            if (i > 0 && list[i]->getx() < list[i - 1]->getx()) {
                particle *temp = list.at(i);
                int j = i - 1;
                while (j >= 0) {
                    if (list.at(j)->getx() > temp->getx()) {
                        list.at(j + 1) = list.at(j);
                        --j;
                    } else {
                        break;
                    }
                }
                list.at(j + 1) = temp;
            }
        }
        else {
                if (i > 0 && list[i]->gety() < list[i - 1]->gety()) {
                    particle *temp = list.at(i);
                    int j = i - 1;
                    while (j >= 0) {
                        if (list.at(j)->gety() > temp->gety()) {
                            list.at(j + 1) = list.at(j);
                            --j;
                        } else {
                            break;
                        }
                    }
                    list.at(j + 1) = temp;
                }
        }
    }
}

bool collisions::testCollision(particle *A, particle *B) {
    double sqDist = (B->getx() - A->getx()) * (B->getx() - A->getx()) + (B->gety() - A->gety()) * (B->gety() - A->gety());
    double sqDiffCenter = (A->getShapeCharacteristicValue() + B->getShapeCharacteristicValue());
    sqDiffCenter *= sqDiffCenter;

//    std::printf("dqdist: %f       sqDiff: %f\n", sqDist, sqDiffCenter);

    if(sqDist <= sqDiffCenter) {
        if (sqDist < sqDiffCenter) {
            //separating the particles
            //Find center separation vector
            double sepX = B->getx() - A->getx();
            double sepY = B->gety() - A->gety();
            double sepMagSq = sepX * sepX + sepY * sepY;
            sepX /= sqrt(sepMagSq);
            sepY /= sqrt(sepMagSq);
            sepX *= (A->getShapeCharacteristicValue() + B->getShapeCharacteristicValue());
            sepY *= (A->getShapeCharacteristicValue() + B->getShapeCharacteristicValue());

            B->setx(A->getx() + sepX);
            B->sety(A->gety() + sepY);
        }
        return true;
    }
    else
        return false;
}

void collisions::checkForCollision() {
    if (collisions::particleListX.size() == 1)
        return;
    int axis = collisions::getParticleList();
    std::vector<particle*> List = (axis)? particleListY : particleListX;

    for(int i = 0; i < List.size() - 1; i++){
        for(int j = i + 1; j < List.size(); j++) {
//            ++totalCalculations;
            if (testCollision(List[i], List[j])) {
                circleParticle::resolveCollision(dynamic_cast<circleParticle *>(List[i]),
                                                 dynamic_cast<circleParticle *>(List[j]));
                ++totalCalculations;
            }
            if(List[j]->getMin()[axis] >= List[i]->getMax()[axis])
                break;
        }
    }
}