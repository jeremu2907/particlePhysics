#include "particle.h"
#include <iostream>

particle::particle(){
    this->x = 0;
    this->y = 0;
    this->vx = 0;
    this->vy = 0;
    this->rotation = 0;
}

particle::particle(double x, double y){
    particle();
    this->x = x;
    this->y = y;
}

particle::particle(double x, double y, double vx, double vy){
    particle();
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
}

particle::particle(double x, double y, double vx, double vy, double rotation){
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
    this->rotation = rotation;
}

double particle::getx(){
    return this->x;
}

double particle::gety(){
    return this->y;
}

double particle::getvx(){
    return this->vx;
}

double particle::getvy(){
    return this->vy;
}

double particle::getrotation(){
    return this->rotation;
}