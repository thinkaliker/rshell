opt=-Wall -Werror -ansi -pedantic
out=./bin/rshell
src=./src/main.cpp
rmb=rm -rf ./bin

all:
	[ -d "./bin" ] && $(rmb)
	mkdir ./bin
	g++ $(opt) $(src) -o $(out)

rshell:
	mkdir ./bin
	g++ $(opt) $(src) -o $(out)

clean:
	$(rmb)
