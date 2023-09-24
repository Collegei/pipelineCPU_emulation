all: main

main: main.o stage.o
	g++ main.o stage.o -o main

main.o:main.cpp
	g++ -c main.cpp -o main.o

stage.o:stage.cpp
	g++ -c stage.cpp -o stage.o

clean:
	rm main *.o
