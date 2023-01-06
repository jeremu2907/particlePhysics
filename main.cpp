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
void testContinuousState(int);
void getNextState();

int main(int argc, char *argv[]){
    cout << "Real Time Particle Collision Engine" << endl << endl;
    testContinuousState(5);
}

void getNextState(){

}

void testContinuousState(int seconds){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("Real-time Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    bool running = true;
    std::vector<particle *> list;
    const float RESTITUTION = 1;

    //Generation list of objects
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> xyDistrib(0, 100);
    std::uniform_int_distribution<> vXDistrib(-30, 30);
    std::uniform_int_distribution<> vYDistrib(-30, 30);
    std::uniform_int_distribution<> mDistrib(1,10);
    for(int j = 0; j <= 100; j += 10) {
        for(int k = 0; k <= 100; k+= 10)
            list.push_back(new circleParticle(k, j, vXDistrib(gen), vYDistrib(gen), mDistrib(gen),RESTITUTION));
    }
//    list.push_back(new circleParticle(70,20, -10, 0,50,RESTITUTION));
//    list.push_back(new circleParticle(50,0,0,0,50));
//    list.push_back(new circleParticle(50,80, 0, 0,50));
//    list.push_back(new circleParticle(50,40,0,0,20));
//    list.push_back(new circleParticle(20,20, 0, 0,1,RESTITUTION));
//    list.push_back(new circleParticle(50,1, 0, 0,10));
    collisions particleList(list);

    timer t_calc;       //Calculate particle state @ 60Hz
    timer t_result;     //To execute something @ 1Hz
    t_calc.start();
    t_result.start();

    double rx = 0;
    double totalLoops = 0;

    std::printf("Simulating...\n");
    for(int i = 0; i < seconds && running;){

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

    std::ofstream outfile("final_state.txt");
    for(auto j : particleList.getList()){
        outfile << j->getvx() << ',' << j->getvy() << '\n';
    }
    outfile.close();

    cout << "\nTotal Collisions: " << particleList.totalCalculations << endl;
    cout << "Total Loops Ran: " << totalLoops << endl;
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