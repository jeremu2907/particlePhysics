#include <iostream>
#include <fstream>
#include "SDL2/SDL.h"
#include "SDL_image.h"
#include <thread>
#include <random>

#include "headers/timer.h"
#include "headers/circleParticle.h"
#include "headers/collisions.h"

using std::cout;
using std::endl;

//Unit Tests
void testScreen();
void testCollisionInit();
void testCollision();
void testContinuousState();

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y)
    {
        //  Each of the following renders an octant of the circle
        SDL_RenderDrawPoint(renderer, centreX + x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX + x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY - y);
        SDL_RenderDrawPoint(renderer, centreX - x, centreY + y);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX + y, centreY + x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY - x);
        SDL_RenderDrawPoint(renderer, centreX - y, centreY + x);

        if (error <= 0)
        {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0)
        {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}

int main(int argc, char *argv[]){
    cout << "Real Time Particle Collision Engine" << endl << endl;
    testContinuousState();
//testScreen();
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
        DrawCircle(renderer, 400,300, 1);
        SDL_RenderPresent(renderer);
        // "Frame" logic
    }
}

void testContinuousState(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("Particle Render", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    bool running = true;
    std::vector<particle *> list;

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> xyDistrib(11, 89);
    std::uniform_int_distribution<> vDistrib(-20, 20);

    for(int j = 0; j < 4; j++)
        list.push_back(new circleParticle(xyDistrib(gen),xyDistrib(gen),vDistrib(gen),vDistrib(gen),1));
    collisions particleList(list);

    timer t_calc;       //Calculate particle state @ 60Hz
    timer t_result;     //To execute something @ 1Hz
    t_calc.start();
    t_result.start();
//
//    std::ofstream outfile1("outfile1.txt");
//    std::ofstream outfile2("outfile2.txt");
//    std::ofstream outfile3("outfile3.txt");
    double rx = 0;

    double totalLoops = 0;

    std::printf("Simulating...\n");
    for(int i = 0; i < 300 && running;){

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
//            cout << "Not passing \n";
            for(auto j : particleList.getList()){
                j->calcSyGravity();
                j->calcSx();
            }
//            cout << "passed\n";

            particleList.checkForCollision();

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

//            cout << "Not passing \n";
            for(auto j : particleList.getList()){
                DrawCircle(renderer,j->getx() / 100 * 800, 800 - (j->gety() / 100 * 800), 64);
            }
//            cout << "passed\n";
            SDL_RenderPresent(renderer);
//            outfile1 << '(' << par1->getx() << ',' << par1->gety() << "),";
//            outfile2 << '(' << par2->getx() << ',' << par2->gety() << "),";
//            outfile3 << '(' << par3->getx() << ',' << par3->gety() << "),";
            t_calc.start();
            rx += (double) 1/60;
            totalLoops++;
        }
    }
//    SDL_RenderClear(renderer);
    running = false;
    std::printf("Simulation Finished\n\n");
    cout << "Total Collisions checked: " << particleList.totalCalculations << endl;
    cout << "Total Loops ran: " << totalLoops << endl;
//    outfile2.close();
//    outfile1.close();
}