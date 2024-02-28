CXX = g++
CXXFLAGS = -std=c++17 -Wall
CXXTESTFLAGS = --coverage
CXXGDB = -ggdb

all: main test

main: Player.o Board.o Game.o main.cpp
	$(CXX) $(CXXFLAGS) main.cpp Player.o Board.o Game.o -o main

test: Player.o Board.o Game.o test.cpp 
	$(CXX) $(CXXFLAGS) $(CXXTESTFLAGS) test.cpp Player.o Board.o Game.o -o test

Player.o: Player.cpp Player.h
	$(CXX) $(CXXFLAGS) -c Player.cpp

Board.o: Board.cpp Board.h
	$(CXX) $(CXXFLAGS) -c Board.cpp

Game.o: Game.cpp Game.h
	$(CXX) $(CXXFLAGS) -c Game.cpp

clean:
	rm -f main test *.o