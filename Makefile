CXX=g++
CXXFLAGS=`sdl2-config --cflags --libs` -Wall -Wextra -std=c++11

snake: bin
	${CXX} -O3 snake.cpp -o bin/snake ${CXXFLAGS}

debug: bin
	${CXX} -g snake.cpp -o bin/debug ${CXXFLAGS}

bin:
	mkdir -p ./bin

clean:
	rm -f bin/*