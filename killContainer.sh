#!/bin/bash

docker kill $(cat ./id)

rm ./id ./CMakeCache.txt

cmake .

rm CMakeCache.txt