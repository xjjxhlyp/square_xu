#!/bin/bash

g++ test/main_test.cpp game.cpp -I ./ -lgtest -lgtest_main -std=c++17 -o ./test.out

if [ $? -eq 0 ]; then
	./test.out
	rm test.out
fi

