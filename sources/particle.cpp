#include "../headers/particle.h"

particle::particle(){
    this->x = 0.0;
    this->y = 0.0;
    this->vx = 0.0;
    this->vy = 0.0;
    this->rotation = 0.0;
    this->mass = 1.0;
}

particle::particle(double x, double y){
    this->x = x;
    this->y = y;
    this->vx = 0.0;
    this->vy = 0.0;
    this->rotation = 0.0;
    this->mass = 1.0;
}

particle::particle(double x, double y, double vx, double vy){
    particle();
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
    this->rotation = 0.0;
    this->mass = 1.0;
}

particle::particle(double x, double y, double vx, double vy, double mass){
    particle();
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
    this->rotation = 0.0;
    this->mass = mass;
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

double particle::getMass(){
    return this->mass;
}

double* particle::getMin(){
    return this->min;
}

double* particle::getMax(){
    return this->max;
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
    double vy0 = this->vy;
    this->vy = G_VAL * DT + vy0;
}

void particle::calcSyGravity(){
    this->y += this->vy * DT + 0.5 * G_VAL * DT * DT;
    if(this->y <= 0){
        this->y *= -1;
        this->vy *= -1;
    } else if (this->y >= 100){
        this->y = 200 - this->y;
        this->vy *= -1;
    } else {
        particle::calcVy();
    }
    this->calcMinMax();
}

void particle::calcSy(){
    this->y += this->vy * DT;
    if(this->y <= 0 ){
        this->y *= -1;
        this->vy *= -1;
    } else if (this->y >= 100){
        this->y = 200 - this->y;
        this->vy *= -1;
    }
    this->calcMinMax();
}

void particle::calcSx(){
    this->x += this->vx * DT;
    if(this->x <= 0){
        this->x *= -1;
        this->vx *= -1;
    } else if (this->x >= 100){
        this->x = 200 - this->x;
        this->vx *= -1;
    }
    this->calcMinMax();
}