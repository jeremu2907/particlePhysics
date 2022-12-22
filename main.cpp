#include <iostream>
#include "timer.h"
#include "circleParticle.h"
#include <fstream>
#include "SDL2/SDL.h"

using std::cout;
using std::endl;


int main(int argc, char *argv[]){

//    SDL_Init(SDL_INIT_VIDEO);
//    SDL_Window * window = SDL_CreateWindow("Particle Render", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
//    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
//    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//    SDL_RenderClear(renderer);
//    SDL_RenderPresent(renderer);
////    SDL_Delay(10000);
//
//    while(true)
//    {
//        SDL_Event e;
//        while (SDL_PollEvent(&e))
//        {
//            if(e.type == SDL_QUIT)
//            {
//                break;
//            }
//            // Handle events
//        }
//
//        // "Frame" logic
//    }

    cout << "Particle Collision Engine" << endl << endl;
    circleParticle * par1 = new circleParticle(-332,32,0, 100);
    std::printf("x = %f  y = %f  min = (%f, %f)  max = (%f, %f)  vx = %f  vy = %f  rotation = %f\n\n",
                par1->getx(), par1->gety(), par1->getMin()[0], par1->getMin()[1],par1->getMax()[0], par1->getMax()[1],
                par1->getvx(), par1->getvy(), par1->getrotation());

    timer t_calc;       //Calculate particle state @ 60Hz
    timer t_result;     //To execute something @ 1Hz
    t_calc.start();
    t_result.start();

    std::ofstream outfile("outfile.txt");
    double rx = 0;

    for(int i = 0; i < 20;){
        if(t_result.isTimeOut(1)){
            std::printf("x = %f  y = %f  min = (%f, %f)  max = (%f, %f)  vx = %f  vy = %f  rotation = %f\n\n",
                        par1->getx(), par1->gety(), par1->getMin()[0], par1->getMin()[1],par1->getMax()[0], par1->getMax()[1],
                        par1->getvx(), par1->getvy(), par1->getrotation());
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
    outfile << std::flush;
    outfile.close();
}