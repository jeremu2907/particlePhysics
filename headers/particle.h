#ifndef PARTICLE_H
#define PARTICLE_H

//Base class for all objects

#include <cmath>
#include <iostream>

class particle{
    protected:
        double x, y, mass;             //Center (x,y), mass
        double vx, vy;    //current and prev Velocity
        double theta;            //angle of rotation
        double va;

        //Calculate min max points of hit box, to be overridden because difference of shapes
        virtual void calcMinMax(){};

        double min[2];      //Holds the bottom left corner of the smallest projection
        double max[2];      //Holds the top right corner of the smallest projection

        void calcVy();
    public:
        enum SHAPE {
            NONE,
            CIRCLE,
            SQUARE,
            TRIANGLE
        };
        static const double G_VAL;
        static const double DT;
        static const double PI;
        static float RESTITUTION;

        particle();
        particle(double x, double y);
        particle(double x, double y, double vx, double vy);
        particle(double x, double y, double vx, double vy, double rotation);
        particle(double x, double y, double vx, double vy, double rotation, float restitution);
        particle(double x, double y, double rotation, double vx, double vy, double va, double mass,float restitution);

        ~particle(){};

        double getx();
        double gety();
        double getvx();
        double getvy();
        double getTheta();
        double getva();
        double getMass();
        virtual double getShapeCharacteristicValue(){
            return 0;
        };   //Depending on what shape it is, it will return its characteristic value (radius, sideLength, etc)

        virtual SHAPE getShape(){
            return NONE;
        };

        double* getMin();
        double* getMax();

        void setx(double e);
        void sety(double e);
        void setvx(double e);
        void setvy(double e);
        void setTheta(double e);
        void setva(double e);

        //Must be inherited because all objects subject to gravity
        void calcSy();
        void calcSyGravity();
        void calcSx();
        void calcTheta();

        double dotV(double x, double y);      //Particle velocity dot a vector [x,y]

};

#endif