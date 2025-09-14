#!/bin/bash


g++dr()
{
  g++ $1 -DDEBUG && ./a.out < in.txt
}


