#ifndef PARTICLE_H
#define PARTICLE_H

#include <cmath>
#include <iostream>

class particle{
    const double G_VAL = - 9.807;
    const double DT = (double)1 / 60;

    private:
        double x, y;
        double vx, vy, vx0, vy0;
        double rotation;

        virtual void xcollision(){};
        virtual void ycollision(){};

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

        void setx(double e);
        void sety(double e);
        void setvx(double e);
        void setvy(double e);
        void setrotation(double e);

        void calcVy();
        void calcSy();
};

#endif