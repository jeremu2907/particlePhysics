cmake_minimum_required(VERSION 3.24)
project(particlePhysics)

set(CMAKE_CXX_STANDARD 14)

set(MY_SOURCES sources/circleParticle.cpp
        headers/circleParticle.h
                main.cpp
        sources/particle.cpp
        headers/particle.h
        headers/timer.h headers/collisions.h sources/collisions.cpp headers/functional.h headers/squareParticle.h sources/squareParticle.cpp)

find_package(SDL2_image REQUIRED)
#target_link_libraries(particlePhysics PRIVATE SDL2_image::SDL2_image)

find_package(SDL2 REQUIRED COMPONENTS SDL2)
add_executable(particlePhysics ${MY_SOURCES})
target_link_libraries(particlePhysics PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)