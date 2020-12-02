

W=4
H=4
L=2

CXX=g++
CXXFLAGS=`sdl2-config --cflags --libs` -Wall -Wextra -std=c++11 -D SNAKE_STATE_BOARD_WIDTH=${W} -D SNAKE_STATE_BOARD_HEIGHT=${H} -D SNAKE_STATE_START_LENGTH=${L}

snake: bin
	${CXX} snake.cpp -o bin/snake ${CXXFLAGS}

debug: bin
	${CXX} -g snake.cpp -o bin/debug ${CXXFLAGS}
bin:
	mkdir -p ./bin

clean:
	rm -f bin/*