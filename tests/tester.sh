#!/bin/sh

g++ -g -c -o main.o main.cpp
g++ -g -c -o MainTestSuite.o MainTestSuite.cpp

g++ main.o \
    MainTestSuite.o \
    -o tester -lnoisetest

./tester
