OUT_PATH=./bin
FILES=State.cpp

W=4
H=4

CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++11 -D SNAKE_STATE_BOARD_WIDTH=${W} -D SNAKE_STATE_BOARD_HEIGHT=${H}

cl_snake: bin
	${CXX} ${CXXFLAGS} -O3 ${FILES} cl_snake.cpp -o bin/cl_snake

debug: bin
	${CXX} ${CXXFLAGS} -g ${FILES} cl_snake.cpp -o bin/debug

bin:
	mkdir -p ./bin

clean:
	rm -f bin/*