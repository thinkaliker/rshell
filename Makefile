opt="-Wall -Werror -ansi -pedantic"

all:
	mkdir ./bin
	g++ ./src/main.cpp  $opt -o ./bin/rshell

rshell:
	mkdir ./bin
	g++ ./src/main.cpp $opt -o ./bin/rshell
