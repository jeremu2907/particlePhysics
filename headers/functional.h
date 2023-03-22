//
// Created by Jeremy Nguyen on 12/29/22.
//

#ifndef PARTICLEPHYSICS_FUNCTIONAL_H
#define PARTICLEPHYSICS_FUNCTIONAL_H

#include "SDL2/SDL.h"
#include <string>

namespace functional{
    const double FREQ = 65.0;
    void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius);
    void DrawSquare(SDL_Renderer * renderer, int32_t centerX, int32_t centerY, int32_t apothem, float rotation);
    void parseStringToData(std::string s, double *x, double *y, double *vx, double *vy, double *m);

    template<typename T>
    T dot(T a[], T b[]);
}

#endif //PARTICLEPHYSICS_FUNCTIONAL_H
