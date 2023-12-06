#include "../headers/particle.h"
#include "../headers/functional.h"

#define _USE_MATH_DEFINES

const double particle::G_VAL = - 9.807;
const double particle::DT = (double)1 / functional::FREQ;
float particle::RESTITUTION = 1;
const double particle::PI = M_PI;

particle::particle()
{
    this->x = 0.0;
    this->y = 0.0;
    this->vx = 0.0;
    this->vy = 0.0;
    this->theta = 0.0;
    this->va = 0.0;
    this->mass = 1.0;
}

particle::particle(double x, double y)
{
    this->x = x;
    this->y = y;
    this->vx = 0.0;
    this->vy = 0.0;
    this->theta = 0.0;
    this->va = 0.0;
    this->mass = 1.0;
}

particle::particle(double x, double y, double vx, double vy)
{
    particle();
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
    this->theta = 0.0;
    this->va = 0.0;
    this->mass = 1.0;
}

particle::particle(double x, double y, double vx, double vy, double mass)
{
    particle();
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
    this->theta = 0.0;
    this->va = 0.0;
    this->mass = mass;
}

particle::particle(double x, double y, double vx, double vy, double mass,float restitution)
{
    particle();
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
    this->theta = 0.0;
    this->va = 0.0;
    this->mass = mass;
    particle::RESTITUTION = restitution;
}

particle::particle(double x, double y, double rotation, double vx, double vy, double va, double mass,float restitution)
{
    particle();
    this->x = x;
    this->y = y;
    this->vx = vx;
    this->vy = vy;
    this->theta = rotation;
    this->va = va;
    this->mass = mass;
    particle::RESTITUTION = restitution;
}

double particle::getx()
{
    return this->x;
}

double particle::gety()
{
    return this->y;
}

double particle::getvx()
{
    return this->vx;
}

double particle::getvy()
{
    return this->vy;
}

double particle::getTheta() 
{
    return this->theta;
}

double particle::getva() 
{
    return this->va;
}

double particle::getMass()
{
    return this->mass;
}

double* particle::getMin()
{
    return this->min;
}

double* particle::getMax()
{
    return this->max;
}

void particle::setx(double e)
{
    this->x = e;
}

void particle::sety(double e)
{
    this->y = e;
}

void particle::setvx(double e)
{
    this->vx = e;
}

void particle::setvy(double e)
{
    this->vy = e;
}

void particle::setTheta(double e) 
{
    this->theta = e;
}

void particle::setva(double e) 
{
    this->va = fmod(e, 2 * particle::PI);
}

void particle::calcVy()
{
    double vy0 = this->vy;
    this->vy = G_VAL * DT + vy0;
}

void particle::calcSyGravity()
{
    this->y += this->vy * DT + 0.5 * G_VAL * DT * DT;
    if(this->y <= 0){
        this->y *= -1;
        this->vy *= -1;
        this->vy *= RESTITUTION;
    } else if (this->y >= 100){
        this->y = 200 - this->y;
        this->vy *= -1;
        this->vy *= RESTITUTION;
    } else {
        particle::calcVy();
    }
    this->calcMinMax();
}

void particle::calcSy()
{
    this->y += this->vy * DT;
    if(this->y <= 0 ){
        this->y *= -1;
        this->vy *= -1;
        this->vy *= RESTITUTION;
    } else if (this->y >= 100){
        this->y = 200 - this->y;
        this->vy *= -1;
        this->vy *= RESTITUTION;
    }
    this->calcMinMax();
}

void particle::calcSx()
{
    this->x += this->vx * DT;
    if(this->x <= 0){
        this->x *= -1;
        this->vx *= -1;
        this->vx *= RESTITUTION;
    } else if (this->x >= 100){
        this->x = 200 - this->x;
        this->vx *= -1;
        this->vx *= RESTITUTION;
    }
    this->calcMinMax();
}

void particle::calcTheta() 
{
    this->theta += this->va * DT;
    if(this->theta > 2 * particle::PI)
        this->theta -= 2 * particle::PI;
    else if (this->theta < -2 * particle::PI)
        this->theta += 2 * particle::PI;
}

double particle::dotV(double x, double y)
{
    return this->getvx() * x + this->getvy() * y;
}