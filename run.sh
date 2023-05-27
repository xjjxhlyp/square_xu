#!/bin/bash

g++ main.cpp game.cpp -I ./ -std=c++17 -o ./game

if [ $? -eq 0 ]; then
	./game
fi

