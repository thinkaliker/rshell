opt="-Wall -Werror -ansi -pedantic"

all:
	mkdir ./bin
	g++ $opt -o ./bin/rshell

rshell:
	mkdir ./bin
	g++ $opt -o ./bin/rshell
