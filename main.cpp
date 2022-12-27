#include <iostream>
#include <fstream>
#include "SDL2/SDL.h"

#include "headers/timer.h"
#include "headers/circleParticle.h"
#include "headers/collisions.h"
#include "headers/particle.h"

using std::cout;
using std::endl;

//Unit Tests
void testScreen();
void testCollisionInit();
void testCollision();
void testContinuousState();


int main(int argc, char *argv[]){
    cout << "Particle Collision Engine" << endl << endl;
    testContinuousState();
}

void testScreen(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("Particle Render", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
//    SDL_Delay(10000);

    while(true)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                break;
            }
            // Handle events
        }

        // "Frame" logic
    }
}
void testCollisionInit(){
    collisions collisionDetect;
    circleParticle * par1 = new circleParticle(1,.26794919999,7, 0,5);
    circleParticle * par2 = new circleParticle(2, 2, -5, 0, 17);
    circleParticle * par3 = new circleParticle(0, -100, 0, 0);
    collisionDetect.addParticle(par2);
    collisionDetect.addParticle(par1);
    collisionDetect.addParticle(par3);

//    for(auto i : collisionDetect.getParticleList())
//        std::printf("x = %f  y = %f  min = (%f, %f)  max = (%f, %f)  vx = %f  vy = %f  rotation = %f\n\n",
//                    i->getx(), i->gety(), i->getMin()[0], i->getMin()[1],i->getMax()[0], i->getMax()[1],
//                    i->getvx(), i->getvy(), i->getrotation());
    delete par1;
    delete par2;
    delete par3;
}
void testCollision(){
    circleParticle * par1 = new circleParticle(0,0,1, 1,4);
    circleParticle * par2 = new circleParticle(2, 0, 0, 1, 2);

    cout << "Before Collision\n";
    std::printf("A:\tvx:%f \tvy:%f \tm:%f\n", par1->getvx(), par1->getvy(), par1->getMass());
    std::printf("B:\tvx:%f \tvy:%f \tm:%f\n", par2->getvx(), par2->getvy(), par2->getMass());
//    circleParticle::resolveCollision(par1, par2);
    circleParticle::resolveCollisionDirect(par1,par2);
    cout << "After Collision\n";
    std::printf("A:\tvx:%f \tvy:%f \tm:%f\n", par1->getvx(), par1->getvy(), par1->getMass());
    std::printf("B:\tvx:%f \tvy:%f \tm:%f\n\n", par2->getvx(), par2->getvy(), par2->getMass());


    delete par1;
    delete par2;
}
void testContinuousState(){
    collisions particleList;

    circleParticle * par1 = new circleParticle(0,25,15, 5,10);
    circleParticle * par2 = new circleParticle(35,25,10, 5,5);
    circleParticle * par3 = new circleParticle(75, 25, -10, 5, 5);
    particleList.addParticle(par1);
    particleList.addParticle(par2);
    particleList.addParticle(par3);

    timer t_calc;       //Calculate particle state @ 60Hz
    timer t_result;     //To execute something @ 1Hz
    t_calc.start();
    t_result.start();

    std::ofstream outfile1("outfile1.txt");
    std::ofstream outfile2("outfile2.txt");
    std::ofstream outfile3("outfile3.txt");
    double rx = 0;

    int totalLoops = 0;

    std::printf("Simulating...\n");
    for(int i = 0; i < 20;){
        if(t_result.isTimeOut(1)){
//            outfile1 << '(' << par1->getx() << ',' << par1->gety() << "),";
//            outfile2 << '(' << par2->getx() << ',' << par2->gety() << "),";
//            outfile3 << '(' << par3->getx() << ',' << par3->gety() << "),";
            t_result.start();
            ++i;
            totalLoops++;
        }

        if(!t_calc.isTimeOut((double)1/60)) {
            //do something to pass time as a busy-wait or sleep
        } else {
            //Calculate state of particle @60Hz
            for(auto j : particleList.getList()){
                j->calcSyGravity();
                j->calcSx();
            }
//            par1->calcSyGravity();
//            par1->calcSx();
//            par2->calcSyGravity();
//            par2->calcSx();

            particleList.checkForCollision();
            rx += (double) 1/60;

            outfile1 << '(' << par1->getx() << ',' << par1->gety() << "),";
            outfile2 << '(' << par2->getx() << ',' << par2->gety() << "),";
            outfile3 << '(' << par3->getx() << ',' << par3->gety() << "),";
            t_calc.start();

            totalLoops++;
        }
    }
    std::printf("Simulation Finished\n\n");
    cout << "Total Collisions checked: " << particleList.totalCalculations << endl;
    cout << "Total Loops ran: " << totalLoops << endl;

    delete par1;
    delete par2;
    outfile2.close();
    outfile1.close();
}