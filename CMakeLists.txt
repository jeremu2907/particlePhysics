cmake_minimum_required(VERSION 3.22)
project(physicsEngine)

set(CMAKE_CXX_STANDARD 14)

set(MY_SOURCES 
        sources/circleParticle.cpp
        headers/circleParticle.h
        sources/particle.cpp
        headers/particle.h
        headers/timer.h
        headers/collisions.h
        sources/collisions.cpp
        headers/functional.h
        sources/functional.cpp
        headers/squareParticle.h
        sources/squareParticle.cpp
        main.cpp)

find_package(SDL2 REQUIRED COMPONENTS SDL2)
find_package(eigen3 REQUIRED COMPONENTS Eigen)
add_executable(physicsEngine ${MY_SOURCES})
target_link_libraries(physicsEngine PRIVATE SDL2::SDL2)