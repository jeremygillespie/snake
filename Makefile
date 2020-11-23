SRC_PATH=./src
OUT_PATH=./bin
INC_PATH=./include

FILES=${SRC_PATH}/*.cpp

CC=g++
CFLAGS=-Wall -std=c++17

cl_snake:
	mkdir -p ${OUT_PATH}
	${CC} ${CFLAGS} -I${INC_PATH} ${FILES} -o ${OUT_PATH}/cl_snake