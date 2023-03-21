#include "../headers/functional.h"

#include <cmath>
#include <iostream>

void functional::DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
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
void functional::DrawSquare(SDL_Renderer * renderer, int32_t centerX, int32_t centerY, int32_t apothem, float rotation){
    //Simplified apothem vector rotated by rotation matrix
    double X_1 = apothem * std::sin(rotation);
    double Y_1 = apothem * std::cos(rotation);

    double X_2 = -1 * Y_1;
    double Y_2 = X_1;

    SDL_RenderDrawLine(renderer, centerX + X_1 + X_2, centerY + Y_1 + Y_2, centerX + X_1 - X_2, centerY + Y_1 - Y_2);
    SDL_RenderDrawLine(renderer, centerX + X_1 - X_2, centerY + Y_1 - Y_2, centerX - X_1 - X_2, centerY - Y_1 - Y_2);
    SDL_RenderDrawLine(renderer, centerX - X_1 - X_2, centerY - Y_1 - Y_2, centerX - X_1 + X_2, centerY - Y_1 + Y_2);
    SDL_RenderDrawLine(renderer, centerX - X_1 + X_2, centerY - Y_1 + Y_2, centerX + X_1 + X_2, centerY + Y_1 + Y_2);
}
void functional::parseStringToData(std::string s, double *x, double *y, double *vx, double *vy, double *m){
    int indexL = 0;
    int indexR = s.find(',', indexL);
    *x = stod(s.substr(indexL,indexR - indexL));

    indexL = ++indexR;
    indexR = s.find(',', indexL);
    *y = stod(s.substr(indexL,indexR - indexL));

    indexL = ++indexR;
    indexR = s.find(',', indexL);
    *vx = stod(s.substr(indexL,indexR - indexL));

    indexL = ++indexR;
    indexR = s.find(',', indexL);
    *vy = stod(s.substr(indexL,indexR - indexL));

    indexL = ++indexR;
    *m = stod(s.substr(indexL,s.length() - indexL));
}

template<typename T>
T functional::dot(T a[], T b[]){
        try{
            if(sizeof(a)/sizeof(a[0]) != sizeof(b)/sizeof(b[0])){
                throw;
            }
        } catch (...) {
            std::cout << "Error input size for dot product\n";
        }

        T res = 0;
        for(int i = 0; i < sizeof(a)/sizeof(a[0]); i++){
            res += a[i] * b[i];
        }

        return res;
    }