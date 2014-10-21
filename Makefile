opt=-Wall -Werror -ansi -pedantic
out=./bin/rshell

all:
	mkdir ./bin
	g++ $(opt) ./src/main.cpp  -o $(out)

rshell:
	mkdir ./bin
	g++ $(opt) ./src/main.cpp  -o $(out)

clean:
	rm -rf ./bin
