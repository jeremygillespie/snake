OUT_PATH=./bin
FILES=CompactState.cpp

W=4
H=4

CXX=g++
CXXFLAGS=-Wall -Wextra -std=c++11 -D SNAKE_STATE_BOARD_WIDTH=${W} -D SNAKE_STATE_BOARD_HEIGHT=${H}

cl_snake_compact: bin
	${CXX} ${CXXFLAGS} -O3 ${FILES} compact_snake.cpp -o bin/cl_snake_compact

debug: bin
	${CXX} ${CXXFLAGS} -g ${FILES} compact_snake.cpp -o bin/debug

bin:
	mkdir -p ./bin

clean:
	rm -f bin/*