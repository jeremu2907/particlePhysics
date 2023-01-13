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
            if (i > 0 && list[i]->getMin()[0] < list[i - 1]->getMin()[0]) {
                particle *temp = list.at(i);
                int j = i - 1;
                while (j >= 0) {
                    if (list.at(j)->getMin()[0] > temp->getMin()[0]) {
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
                if (i > 0 && list[i]->getMin()[1] < list[i - 1]->getMin()[1]) {
                    particle *temp = list.at(i);
                    int j = i - 1;
                    while (j >= 0) {
                        if (list.at(j)->getMin()[1] > temp->getMin()[1]) {
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

double* collisions::intermediateV(double v1, double v2, double m1, double m2) {
    double sumM = m1 + m2;
    double p = v1 * m1 + v2 * m2;
    double v_1 = (p + (v2-v1) * m2)/sumM;
    double v_2 = v1+v_1-v2;

    return new double[2] {v_1, v_2};
}

void collisions::resolveCollisionCircleCircle(circleParticle *A, circleParticle *B) {
//    //separating the particles
//    //Find center separation vector
//    double sepX = B->getx() - A->getx();
//    double sepY = B->gety() - A->gety();
//    double sepMagSq = sepX * sepX + sepY * sepY;
//    sepX /= sqrt(sepMagSq);
//    sepY /= sqrt(sepMagSq);
//    sepX *= (A->getShapeCharacteristicValue() + B->getShapeCharacteristicValue());
//    sepY *= (A->getShapeCharacteristicValue() + B->getShapeCharacteristicValue());

    //Find Tangent vector
    double tanX = - (B->gety() - A->gety());
    double tanY = B->getx() - A->getx();
    double magTanSq = tanX * tanX + tanY * tanY;

    //Find dot product of component B/A in respect to tan vector
    double ADotTan = A->getvx() * tanX + A->getvy() * tanY;
    double BDotTan = B->getvx() * tanX + B->getvy() * tanY;
    //Parallel vector of AB in the direction of tan vector
    double paraAX = (ADotTan / (magTanSq)) * tanX;
    double paraAY = (ADotTan / (magTanSq)) * tanY;
    double paraBX = (BDotTan / (magTanSq)) * tanX;
    double paraBY = (BDotTan / (magTanSq)) * tanY;

    //Calculating perpendicular vector in respect to tan vector, taking into account mass to conservee momentum
    //Momentum is conserved in its own respective axis (DO NOT CHECK BY DOING PYTHAGOREAN THEOREM BC IT WILL SEEM WRONG
    //Function out put the change in V_axis [vA, vB]
    double perpAX = A->getvx() - paraAX;
    double perpAY = A->getvy() - paraAY;
    double perpBX = B->getvx() - paraBX;
    double perpBY = B->getvy() - paraBY;
    double* X = collisions::intermediateV(perpAX, perpBX, A->getMass(), B->getMass());
    double* Y = collisions::intermediateV(perpAY, perpBY, A->getMass(), B->getMass());

    A->setvx(paraAX + X[0]);
    if(X[0] > 0.000000001) A->setvx(A->getvx() * particle::RESTITUTION);
    A->setvy(paraAY + Y[0]);
    if(Y[0] > 0.000000001) A->setvy(A->getvy() * particle::RESTITUTION);
    B->setvx(paraBX + X[1]);
    if(X[1] > 0.000000001) B->setvx(B->getvx() * particle::RESTITUTION);
    B->setvy(paraBY + Y[1]);
    if(Y[1] > 0.000000001) B->setvy(B->getvy() * particle::RESTITUTION);

    delete X;
    delete Y;
}

bool collisions::testCollisionCircleCircle(particle *A, particle *B) {
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

void collisions::orderSquareCircle(particle **A, particle **B) {
    if((*B)->getShape() == particle::SQUARE) {
        particle *temp = *A;
        *A = *B;
        *B = temp;
    }
}

bool collisions::testCollisionSquareCircle(particle& p1, particle& p2) {
    //Make A be the square particle and B circular for standardization
//    std::cout << p1 << std::endl << p2;
    particle *A = &p1;
    particle *B = &p2;
//    if(p1.getShape() == particle::SQUARE) {
//        A = &p1;
//        B = &p2;
//    } else {
//        B = &p1;
//        A = &p2;
//    }
    collisions::orderSquareCircle(&A,&B);

    //Directional vectors for rotation indication
    double X_1 = -1 * std::sin(A->getTheta());
    double Y_1 = std::cos(A->getTheta());
    double X_2 = -1 * Y_1;
    double Y_2 = X_1;

    double uX = B->getx() - A->getx();
    double uY = B->gety() - A->gety();

    //Dot product with unit vector is the same as the magnitude of projection vector
    double uXDot_1 = std::abs(uX * X_1 + uY * Y_1);
    double uXDot_2 = std::abs(uX * X_2 + uY * Y_2);

    double L = A->getShapeCharacteristicValue() + B->getShapeCharacteristicValue();

    if (uXDot_1 > L || uXDot_2 > L){
        return false;
    }
    return true;
}

void collisions::checkForCollision() {
    if (collisions::particleListX.size() == 1)
        return;
    int axis = collisions::getParticleList();
    std::vector<particle*> List = (axis)? particleListY : particleListX;

    for(int i = 0; i < List.size() - 1; i++){
        for(int j = i + 1; j < List.size(); j++) {

            //Both paticles are circular
            if(List[i]->getShape() == particle::CIRCLE && List[j]->getShape() == particle::CIRCLE) {
                if (testCollisionCircleCircle(List[i], List[j])) {
                    collisions::resolveCollisionCircleCircle(dynamic_cast<circleParticle *>(List[i]),
                                                     dynamic_cast<circleParticle *>(List[j]));
                    ++totalCalculations;
                }
            }

            else if((List[i]->getShape() == particle::CIRCLE && List[j]->getShape() == particle::SQUARE) ||
                    (List[i]->getShape() == particle::SQUARE && List[j]->getShape() == particle::CIRCLE) ){
                if (testCollisionSquareCircle(*List[i], *List[j])) {
                    std::cout << "Collided"<< std::endl;

                }
            }

            if (List[j]->getMin()[axis] >= List[i]->getMax()[axis])
                break;
        }
    }
}