cmake_minimum_required(VERSION 3.22)
project(physicsEngine)

set(CMAKE_CXX_STANDARD 14)

set(MY_SOURCES 
        headers/functional.h
        sources/functional.cpp
        sources/circleParticle.cpp
        headers/circleParticle.h
        sources/particle.cpp
        headers/particle.h
        headers/timer.h
        headers/collisions.h
        sources/collisions.cpp
        headers/squareParticle.h
        sources/squareParticle.cpp
        main.cpp)

find_package(SDL2 REQUIRED COMPONENTS SDL2)
add_executable(physicsEngine ${MY_SOURCES})
target_link_libraries(physicsEngine PRIVATE SDL2::SDL2)