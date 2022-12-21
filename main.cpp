#include <iostream>
#include "particle.h"

using std::cout;
using std::endl;

int main(){
    cout << "Particle Collision Engine" << endl << endl;
    particle * par1 = new particle(12,23);
    std::printf("x = %f  y = %f  vx = %f  vy = %f  rotation = %f\n\n", par1->getx(), par1->gety(), par1->getvx(), par1->getvy(), par1->getrotation());
    delete par1;
    par1 = new particle(40, 23, 33, 53);
    std::printf("x = %f  y = %f  vx = %f  vy = %f  rotation = %f\n\n", par1->getx(), par1->gety(), par1->getvx(), par1->getvy(), par1->getrotation());
    delete par1;
    par1 = new particle(123,542,6,2,523);
    std::printf("x = %f  y = %f  vx = %f  vy = %f  rotation = %f\n\n", par1->getx(), par1->gety(), par1->getvx(), par1->getvy(), par1->getrotation());
    delete par1;
}