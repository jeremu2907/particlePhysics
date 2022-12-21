#include <iostream>
#include "particle.h"
#include "timer.h"
#include "circleParticle.h"
#include <fstream>

using std::cout;
using std::endl;

int main(){
    cout << "Particle Collision Engine" << endl << endl;
    circleParticle * par1 = new circleParticle(-332,32,0, 100);
    std::printf("x = %f  y = %f  vx = %f  vy = %f  rotation = %f\n\n", par1->getx(), par1->gety(), par1->getvx(), par1->getvy(), par1->getrotation());

    timer t_calc;       //Calculate particle state @ 60Hz
    timer t_result;     //To execute something @ 1Hz
    t_calc.start();
    t_result.start();

    std::ofstream outfile("outfile.txt");
    double rx = 0;

    for(int i = 0; i < 20;){
        if(t_result.isTimeOut(1)){
            std::printf("x = %f  y = %f  vx = %f  vy = %f  rotation = %f\n\n", par1->getx(), par1->gety(), par1->getvx(), par1->getvy(), par1->getrotation());
            outfile << '(' << i << ',' << par1->gety() << "),\n";
            t_result.start();
            ++i;
        }

        if(!t_calc.isTimeOut((double)1/60)) {
            //do something to pass time as a busy-wait or sleep
        } else {
            //Calculate state of particle @60Hz
            par1->calcSy();
            par1->calcVy();
            rx += + (double) 1/60;

            // outfile << '(' << rx << ',' << par1->gety() << "),";
            t_calc.start();
        }
    }

    delete par1;
    outfile.close();
}