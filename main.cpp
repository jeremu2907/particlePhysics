#include <iostream>
#include <fstream>
#include "SDL2/SDL.h"
#include <mutex>
#include <thread>
#include <random>
#include <condition_variable>

#include "headers/timer.h"
#include "headers/circleParticle.h"
#include "headers/collisions.h"
#include "headers/functional.h"
#include "headers/squareParticle.h"

using std::cout;
using std::endl;
using std::cin;

//Unit Tests
void testScreen();
void testContinuousState(int);
int getNextState(std::string, float);
void waitForInput();
void nextStateRender(std::string);

std::mutex m;
std::condition_variable cv;

int main(int argc, char *argv[]){
    cout << "Real Time Particle Collision Engine" << endl << endl;

//    std::thread renderScreen(nextStateRender,"next_state.txt");

//    getNextState("next_state.txt",1);
    testContinuousState(1200);
//    renderScreen.join();
//    testScreen();
}

int getNextState(std::string stateFile, float RESTITUTION){
    std::ifstream inFile(stateFile);
    collisions particleList;

    double x = -1;
    double y= -1;
    double vx= -1;
    double vy= -1;
    double mass = 0;
    std::string s;

    while(inFile >> s){
        functional::parseStringToData(s, &x, &y, &vx, &vy, &mass);
        particleList.addParticle(new circleParticle(x,y,vx,vy,mass,RESTITUTION));
    }

    if(particleList.getList().size() == 0){
        cout << "Empty Input File";
        return 1;
    }

    int n = 10;
    inFile.close();
    while(true) {
        std::ofstream outFile(stateFile);
        particleList.updateParticle();
        particleList.checkForCollision();
        for (auto i: particleList.getList()) {
            i->calcSx();
            i->calcSy();
            outFile << i->getx() << ',' << i->gety() << ',' << i->getvx() << ',' << i->getvy() << ',' << i->getMass()
                    << '\n';
        }
        outFile.close();
        cin.get();
    }
    return 0;
}

void testContinuousState(int seconds){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window * window = SDL_CreateWindow("Real-time Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    bool running = true;
    std::vector<particle *> list;

    //Generation list of objects
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> xyDistrib(1, 99);
    std::uniform_int_distribution<> vXDistrib(-20, 20);
    std::uniform_int_distribution<> vYDistrib(-20, 20);
    std::uniform_int_distribution<> mDistrib(1,10);
    std::uniform_int_distribution<> vaDistrib(-100, 100);

    //Thermo Dynamics
//    const float RESTITUTION = 0.97;
//    for(int j = 0; j <= 100; j += 5) {
//        for(int k = 0; k <= 100; k+= 5){
//            if (k > 50)
//                list.push_back(new circleParticle(k, j, vXDistrib(gen), vYDistrib(gen), mDistrib(gen),RESTITUTION));
//            else
//                list.push_back(new circleParticle(k, j, 0, 0, mDistrib(gen),RESTITUTION));
//        }
//    }

//    Sound wave
//    const float RESTITUTION = 0.9;
//    for(int j = 0; j <= 100; j += 3) {
//        for(int k = 0; k <= 100; k+= 3){
//            if (k > 49 && k < 56)
//                list.push_back(new circleParticle(k, j, -50, 0, 1,RESTITUTION));
//            else
//                list.push_back(new circleParticle(k, j, 0, 0, 1,RESTITUTION));
//        }
//    }

    const float RESTITUTION = 1;
    for(int j = 0; j <= 100; j +=30) {
        for(int k = 0; k <= 100; k+= 30){
                list.push_back(new circleParticle(k, j, vXDistrib(gen), vYDistrib(gen), mDistrib(gen),RESTITUTION));
        }
    }

    squareParticle * square = new squareParticle(75,75,0,vXDistrib(gen),vYDistrib(gen),particle::PI,mDistrib(gen),RESTITUTION);
    list.push_back(square);

//    list.push_back(new squareParticle(45,50,0,10,0,vaDistrib(gen) * 0 / 100.0 * particle::PI,100,1));
//    list.push_back(new circleParticle(70, 50, -10, 0, 100,RESTITUTION));
//    list.push_back(new squareParticle(50,50,0,0,0,1,100,1));


//    list.push_back(new circleParticle(50,50,10,10,50,1));
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
                if(j->getShape() == particle::SQUARE)
                    j->calcTheta();
            }

//            square->calcSy();
//            square->calcSx();
//            square->calcTheta();

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

            for(auto j : particleList.getList()){
                if(j->getShape() == particle::CIRCLE)
                    functional::DrawCircle(renderer,j->getx() / 100 * 800, 800 - (j->gety() / 100 * 800), j->getShapeCharacteristicValue() * 8);
                else if(j->getShape() == particle::SQUARE)
                    functional::DrawSquare(renderer, j->getx()/ 100 * 800, 800 - (j->gety() / 100 * 800),j->getShapeCharacteristicValue() * 8, j->getTheta());
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
        outfile << j->getx() << ',' << j->gety() << ',' << j->getvx() << ',' << j->getvy() << ',' << j->getMass() << '\n';
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
        functional::DrawSquare(renderer,250,250, 10, 0.12);
        SDL_RenderPresent(renderer);
        // "Frame" logic
    }
}