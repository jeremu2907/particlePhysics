#ifndef PARTICLE_H
#define PARTICLE_H

class particle{
    private:
        double x, y;
        double vx, vy;
        double rotation;
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

        // void setx(double e){};
        // void sety(double e){};
        // void setvx(double e){};
        // void setvy(double e){};
        // void setrotation(double e){};
};

#endif