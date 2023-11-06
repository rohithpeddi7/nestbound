all: compile link execute

compile:
	g++ tp1.cpp -c

link:
	g++ tp1.o -o tp1 -lGL -lGLU -lglut -lSOIL 

execute:
	./tp1