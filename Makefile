opt=-Wall -Werror -ansi -pedantic
out=./bin/rshell

all:
	mkdir ./bin
	g++ $(opt) ./src/main.cpp  -o $(out)/rshell

rshell:
	mkdir ./bin
	g++ $(opt) ./src/main.cpp  -o $(out)/rshell

clean:
	rm -rf ./bin
