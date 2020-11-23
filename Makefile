OUT_PATH=./bin
FILES=State.cpp

CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++11

cl_snake: bin
	${CXX} ${CXXFLAGS} ${FILES} cl_snake.cpp -o bin/cl_snake

minmax: bin
	${CXX} ${CXXFLAGS} ${FILES} minmax.cpp -o bin/minmax

bin:
	mkdir -p ./bin