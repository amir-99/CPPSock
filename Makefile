# Hello World project Makefile

COMPILER=g++
LINKER=g++

# Compile Options
STDCPP=c++14
OPT=-g -pthread

# OS specefic
ifeq ($(OS), Windows_NT)
	RM=del
	CP=copy
else
	RM=rm
	CP=cp
endif


# Targets

default: all

all: exec

exec: exec.o
	${LINKER}  ClientHandler.o TCPServer.o exec.o -o exec.exe -l wsock32

exec.o: exec.cpp TCPServer.o
	${COMPILER} -std=${STDCPP} ${OPT} -c exec.cpp -o exec.o

TCPServer.o: TCPServer.cpp ClientHandler.o
	${COMPILER} -std=${STDCPP} ${OPT} -c TCPServer.cpp -o TCPServer.o

ClientHandler.o: ClientHandler.cpp
	${COMPILER} -std=${STDCPP} ${OPT} -c ClientHandler.cpp -o ClientHandler.o

clean:
	${RM} *.o *.exe
