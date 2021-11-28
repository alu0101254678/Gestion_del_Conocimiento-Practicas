CXX := g++
CXXFLAGS := -g -Wall -O0 -Wextra
SOURCE := ./src/*.cc
BIN	:= bin
EXECUTABLE	:= main

all:
	$(CXX) $(CXXFLAGS) $(SOURCE) -o ./$(BIN)/$(EXECUTABLE)

clean: 
	rm -f ./$(BIN)/*
	cat /dev/null > matriz_utilidad.txt