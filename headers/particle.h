#ifndef PARTICLE_H
#define PARTICLE_H

//Base class for all objects

#include <cmath>
#include <iostream>

class particle{
    const double G_VAL = - 9.807;
    const double DT = (double)1 / 60;

    protected:
        double x, y, mass;             //Center (x,y), mass
        double vx, vy;    //current and prev Velocity
        double rotation;            //angle of rotation

        //Calculate min max points of hit box, to be overridden because difference of shapes
        virtual void calcMinMax(){};

        double min[2];      //Holds the bottom left corner of the smallest projection
        double max[2];      //Holds the top right corner of the smallest projection

        void calcVy();
    public:
        particle();
        particle(double x, double y);
        particle(double x, double y, double vx, double vy);
        particle(double x, double y, double vx, double vy, double rotation);

        ~particle(){};

        double getx();
        double gety();
        double getvx();
        double getvy();
        double getrotation();
        double getMass();
        virtual double getShapeCharacteristicValue(){
            return 0;
        };   //Depending on what shape it is, it will return its characteristic value (radius, sideLength, etc)

        double* getMin();
        double* getMax();

        void setx(double e);
        void sety(double e);
        void setvx(double e);
        void setvy(double e);
        void setrotation(double e);

        //Must be inherited because all objects subject to gravity
        void calcSy();
        void calcSyGravity();
        void calcSx();

};

#endif