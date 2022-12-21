#include "particle.h"
#include <iostream>

particle::particle(){
    this->x = 0.0;
    this->y = 0.0;
    this->vx = 0.0;
    this->vx0 = 0.0;
    this->vy = 0.0;
    this->vy0 = 0.0;
    this->rotation = 0.0;
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
    this->vx0 = vx;
    this->vy0 = vy;
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

void particle::setx(double e){
    this->x = e;
}

void particle::sety(double e){
    this->y = e;
}

void particle::setvx(double e){
    this->vx = e;
}

void particle::setvy(double e){
    this->vy = e;
}

void particle::setrotation(double e){
    this->rotation = e;
}

void particle::calcVy(){
    this->vy0 = this->vy;
    this->vy = G_VAL * DT + vy0;
    // std::cout << G_VAL * DT << std::endl;
}

void particle::calcSy(){
    this->y += this->vy0 * DT + 0.5 * G_VAL * DT * DT;
    // std::cout << this->y;
}