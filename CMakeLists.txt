cmake_minimum_required(VERSION 3.24)
project(particlePhysics)

set(CMAKE_CXX_STANDARD 14)

set(MY_SOURCES circleParticle.cpp
                circleParticle.h
                main.cpp
                particle.cpp
                particle.h
                timer.h)

find_package(SDL2 REQUIRED COMPONENTS SDL2)
add_executable(particlePhysics ${MY_SOURCES})
target_link_libraries(particlePhysics PRIVATE SDL2::SDL2)