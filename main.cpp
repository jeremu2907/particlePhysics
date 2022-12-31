#include <iostream>
#include <fstream>
#include "SDL2/SDL.h"
#include <thread>
#include <random>

#include "headers/timer.h"
#include "headers/circleParticle.h"
#include "headers/collisions.h"
#include "headers/functionals.h"

using std::cout;
using std::endl;

//Unit Tests
void testScreen();
void testContinuousState();

int main(int argc, char *argv[]){
    cout << "Real Time Particle Collision Engine" << endl << endl;
    testContinuousState();
}

void testContinuousState(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("Real-time Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    bool running = true;
    std::vector<particle *> list;
    const float RESTITUTION = 0.8;

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> xyDistrib(0, 100);
    std::uniform_int_distribution<> vDistrib(-20, 20);
    std::uniform_int_distribution<> mDistrib(1,3);

//    for(int j = 0; j <= 100; j += 20) {\
//        for(int k = 0; k <= 100; k += 20)
//            list.push_back(new circleParticle(j, k, vDistrib(gen), vDistrib(gen), mDistrib(gen), RESTITUTION));
//    }
    list.push_back(new circleParticle(30,60, -20, 0,5, RESTITUTION));
    list.push_back(new circleParticle(70,60,0,0,1,RESTITUTION));
    list.push_back(new circleParticle(40,60, 0, 0,1,RESTITUTION));
    list.push_back(new circleParticle(50,60,0,0,1,RESTITUTION));
    list.push_back(new circleParticle(80,60, 0, 0,1,RESTITUTION));
    collisions particleList(list);

    timer t_calc;       //Calculate particle state @ 60Hz
    timer t_result;     //To execute something @ 1Hz
    t_calc.start();
    t_result.start();

    double rx = 0;

    double totalLoops = 0;

    std::printf("Simulating...\n");
    for(int i = 0; i < 100 && running;){

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                running = false;
                break;
            }
            // Handle events
        }

        if(t_result.isTimeOut(1)){
            t_result.start();
            ++i;
            totalLoops++;
        }

        if(!t_calc.isTimeOut((double)1/60)) {
            //do something to pass time as a busy-wait or sleep
        } else {
            //Calculate state of particle @60Hz
            particleList.updateParticle();
            particleList.checkForCollision();

            for(auto j : particleList.getList()){
                j->calcSy();
                j->calcSx();
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            for(auto j : particleList.getList()){
                functionals::DrawCircle(renderer,j->getx() / 100 * 800, 800 - (j->gety() / 100 * 800), j->getShapeCharacteristicValue() * 8);
            }
            SDL_RenderPresent(renderer);

            t_calc.start();
            rx += (double) 1/60;
            totalLoops++;
        }
    }
//    SDL_RenderClear(renderer);
    running = false;
    std::printf("Simulation Finished\n\n");
    std::printf("Final States: \n");
    double netVX = 0;
    double netVY = 0;
    for(auto j : particleList.getList()){
        netVX += j->getvx();
        netVY += j->getvy();
        std::printf("vx: %f \t vy:%f\n", j->getvx(), j->getvy());
    }
    std::printf("Total vx: %f \t vy:%f\n", netVX, netVY);
    cout << "\nTotal Collisions checked: " << particleList.totalCalculations << endl;
    cout << "Total Loops ran: " << totalLoops << endl;
//    outfile2.close();
//    outfile1.close();
}

void testScreen(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("Particle Render", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    bool running = true;

    int x = 0;
    int y = 0;

    while(running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if(e.type == SDL_QUIT)
            {
                running = false;
                break;
            }
            // Handle events
        }
        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
        SDL_RenderDrawLine(renderer, x++ % 800,y++ % 600, (x++ + 100) % 800,(y++ + 100) % 600);
        functionals::DrawCircle(renderer, 400,300, 1);
        SDL_RenderPresent(renderer);
        // "Frame" logic
    }
}