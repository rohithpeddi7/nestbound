all: compile link execute

compile:
	g++ nestbound.cpp -c

link:
	g++ nestbound.o -o nestbound -lGL -lGLU -lglut -lSOIL 

execute:
	./nestbound