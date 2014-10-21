opt=-Wall -Werror -ansi -pedantic
out=./bin/rshell

all:
	[ -d "./bin" ] && rm -rf ./bin
	mkdir ./bin
	g++ $(opt) ./src/main.cpp  -o $(out)

rshell:
	mkdir ./bin
	g++ $(opt) ./src/main.cpp  -o $(out)

clean:
	rm -rf ./bin
