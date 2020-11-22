NAME=snake

SRC_PATH=./src
OUT_PATH=./bin
INC_PATH=./include

FILES=${SRC_PATH}/*.cpp

CC=g++
CFLAGS=-Wall -std=c++11

all:
	mkdir -p ${OUT_PATH}
	${CC} ${CFLAGS} -I${INC_PATH} ${FILES} -o ${OUT_PATH}/${NAME}