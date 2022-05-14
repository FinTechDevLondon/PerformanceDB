#!/usr/bin/env bash

code=0

if [ $code -eq 0 ]
then
    g++ -Wfatal-errors -c dbdata.cpp -o dbdata.o -std=c++20
    code=$?
fi;

if [ $code -eq 0 ]
then
    g++ -Wfatal-errors -c dbtable.cpp -o dbtable.o -std=c++20
    code=$?
fi;

if [ $code -eq 0 ]
then
    g++ -Wfatal-errors -c main.cpp -o main.o -std=c++20
    code=$?
fi;

if [ $code -eq 0 ]
then
    g++ -Wfatal-errors main.o dbdata.o dbtable.o -o a.out
    code=$?
fi;