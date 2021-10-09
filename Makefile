# Hello World project Makefile

COMPILER=g++
LINKER=g++

# Compile Options
STDCPP=c++14
OPT=-g

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

all: demo_client exec

# Client Top Level Target : demo_client

demo_client: demo_client.o
	${LINKER}  demo_client.o -o demo_client.exe -l wsock32

demo_client.o: demo_client.cpp
	${COMPILER} -std=${STDCPP} ${OPT} -c demo_client.cpp -o demo_client.o

# Server Top Level Target : exec
# lib "server" : server.hpp server.cpp

exec: exec.o
	${LINKER}  exec.o server.o -o exec.exe -l wsock32

exec.o: exec.cpp server.o
	${COMPILER} -std=${STDCPP} ${OPT} -c exec.cpp -o exec.o

server.o: server.cpp server.hpp
	${COMPILER} -std=${STDCPP} ${OPT} -c server.cpp -o server.o

clean:
	${RM} *.o *.exe
