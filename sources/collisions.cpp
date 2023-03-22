//
// Created by Jeremy Nguyen on 12/22/22.
//

#include "../headers/collisions.h"
#include "../headers/functional.h"

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

void collisions::resolveCollisionSquareCircle(particle *A, particle *B) {
    collisions::orderSquareCircle(&A, &B);

    //Directional unit vectors for rotation indication
    double X_1 = -std::sin(A->getTheta());
    double Y_1 = std::cos(A->getTheta());
    double X_2 = -Y_1;
    double Y_2 = X_1;
    double vCenterX = B->getx() - A->getx();
    double vCenterY = B->gety() - A->gety();

    //Switches value of 1 to 2 if center vector dot 1 is less than center dot 2
    if(std::abs(vCenterX * X_1 + vCenterY * Y_1) < abs(vCenterX * X_2 + vCenterY * Y_2)){
        double temp;
        temp = X_1;
        X_1 = X_2;
        X_2 = temp;
        temp = Y_1;
        Y_1 = Y_2;
        Y_2 = temp;
    }

    //X_1 will be the vector for perp / contact line norm
    //X_2 will be the vector for parallel / contact line

	////////////////////////////////////////////////
	//Calculating square rotation v at contact point
	////////////////////////////////////////////////

	//Contact point vector
    // double vCenterDotParallel = (double) functional::dot({vCenterX, vCenterY}, {X_2, Y_2});
    double vCenterDotParallel = vCenterX*X_2 + vCenterY*Y_2;
	double v_contact_parallel[2] = {vCenterDotParallel * X_2, vCenterDotParallel * Y_2};
    double v_contact_perpendicular[2] = {A->getShapeCharacteristicValue() * X_1, A->getShapeCharacteristicValue() * Y_1};
    if(vCenterX*X_1 + vCenterY*Y_1 <= 0){
        v_contact_perpendicular[0] *= -1;
        v_contact_perpendicular[1] *= -1;
    }

    double v_contact_vector[2] = {v_contact_parallel[0] + v_contact_perpendicular[0],v_contact_parallel[1] + v_contact_perpendicular[1]};
	double mag_v_contact_vectorSq = v_contact_vector[0]*v_contact_vector[0] + v_contact_vector[1]*v_contact_vector[1];

    //Tangent rotation vector, at contact point
    double v_tan_veclocity[2] = {-1 * v_contact_vector[1] * A->getva(), v_contact_vector[0] * A->getva()};
    double v_tan_velocityDotPerp = v_tan_veclocity[0] * X_1 + v_tan_veclocity[1] * Y_1;
    double v_tan_veclocity_impact[2] = {v_tan_velocityDotPerp * X_1, v_tan_velocityDotPerp * Y_1};


	////////////////////////////////////////////////
	//Calculating translation v of both
	////////////////////////////////////////////////

    //Caculating circular object v's
    double vcDot = B->getvx() * X_1 + B->getvy() * Y_1;
    double vcPerpX = vcDot * X_1;
    double vcPerpY = vcDot * Y_1;
    double vcParaX = B->getvx() - vcPerpX;
    double vcParaY = B->getvy() - vcPerpY;

    //Calculating square tranlational v's
    double vsDot = A->getvx() * X_1 + A->getvy() * Y_1;
    double vsPerpX = vsDot * X_1;
    double vsPerpY = vsDot * Y_1;
    double vsParaX = A->getvx() - vsPerpX;
    double vsParaY = A->getvy() - vsPerpY;

    ////////////////////////////////////////////////
    //Resolving collision
    ////////////////////////////////////////////////

    //General calculation, no rotation yetd
    double* X = collisions::intermediateV(vsPerpX + v_tan_veclocity_impact[0], vcPerpX, A->getMass(), B->getMass());
    double* Y = collisions::intermediateV(vsPerpY + v_tan_veclocity_impact[1], vcPerpY, A->getMass(), B->getMass());

    double energyBefore = A->getMass() * (A->getvx()*A->getvx() + A->getvy()*A->getvy()) + 
                          B->getMass() * (B->getvx()*B->getvx() + B->getvy()*B->getvy()) + 
                    A->getva() * A->getva() / 6.0 * A->getMass() * A->getShapeCharacteristicValue() * A->getShapeCharacteristicValue();
    energyBefore *= 0.5;

    //Translation v
    double resultant_vsDotContact = X[0] * v_contact_vector[0] + Y[0] * v_contact_vector[1];
    double t_v[2] = {X[0], Y[0]};
    X[0] = resultant_vsDotContact * v_contact_vector[0] / mag_v_contact_vectorSq;
    Y[0] = resultant_vsDotContact * v_contact_vector[1] / mag_v_contact_vectorSq;

    A->setvx((vsParaX + X[0]) * particle::RESTITUTION);
    A->setvy((vsParaY + Y[0]) * particle::RESTITUTION);
    B->setvx((vcParaX + X[1]) * particle::RESTITUTION);
    B->setvy((vcParaY + Y[1]) * particle::RESTITUTION);

    double energyAfter = A->getMass() * (A->getvx()*A->getvx() + A->getvy()*A->getvy()) + 
                         B->getMass() * (B->getvx()*B->getvx() + B->getvy()*B->getvy());
    energyAfter *= 0.5;

    double energyDiff = abs(energyBefore - energyAfter);
    std::cout << energyDiff << std::endl;
    double va = sqrt(2 * energyDiff / (1.0/6.0 * A->getMass() * A->getShapeCharacteristicValue() * A->getShapeCharacteristicValue())); 
    
    std::cout << va << std::endl;

    double rotation_v[2] = {t_v[0] - X[0], t_v[1] - Y[0]};
    if(rotation_v[0] * v_tan_veclocity[0] + rotation_v[1] * v_tan_veclocity[1] > 0){
        va *= -1;
    }

    A->setva(va * particle::RESTITUTION);
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
    particle *A = &p1;
    particle *B = &p2;

    collisions::orderSquareCircle(&A,&B);

    //Directional vectors for rotation indication
    double X_1 = -std::sin(A->getTheta());
    double Y_1 = std::cos(A->getTheta());
    double X_2 = -Y_1;
    double Y_2 = X_1;

    double uX = B->getx() - A->getx();
    double uY = B->gety() - A->gety();

    //Dot product with unit vector is the same as the magnitude of projection vector
    double uXDot_1 = std::abs(uX * X_1 + uY * Y_1);
    double uXDot_2 = std::abs(uX * X_2 + uY * Y_2);

    double L = A->getShapeCharacteristicValue() + B->getShapeCharacteristicValue();

    //Not colliding
    if (uXDot_1 > L || uXDot_2 > L){
        return false;
    }

    //Collides
    //Separating particles
    double uMag = sqrt(uX*uX + uY*uY);
    uX /= uMag;
    uY /= uMag;
    double uDotV1 = std::abs(uX * X_1 + uY * Y_1);
    double uDotV2 = std::abs(uX * X_2 + uY * Y_2);
    double scale = (L + 0.05) / ((uDotV1 > uDotV2)? uDotV1 : uDotV2);
    double sepX = scale * uX;
    double sepY = scale * uY;

    B->setx(A->getx() + sepX);
    B->sety(A->gety() + sepY);

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
                    collisions::resolveCollisionSquareCircle(List[i], List[j]);
                }
            }

            if (List[j]->getMin()[axis] >= List[i]->getMax()[axis])
                break;
        }
    }
}
