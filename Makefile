CC := g++
FLAGS := -std=c++17 -Wall -Wextra -pedantic-errors -ggdb 

all : main
	./main FCFS Trace.txt

main : main.o Computer.o
	$(CC) main.o Computer.o -o main

main.o : main.cpp
	$(CC) $(FLAGS) -c main.cpp

Computer.o : Computer.cpp
	$(CC) $(FLAGS) -c Computer.cpp
