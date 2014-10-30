DIRECTORY=./bin
opt=-Wall -Werror -ansi -pedantic
rshellout=./bin/rshell
rshellsrc=./src/main.cpp
lsout=./bin/ls
lssrc=./src/ls.cpp
rmb=rm -rf ./bin

all:
	if [ -d "$(DIRECTORY)" ]; then  $(rmb); fi
	mkdir ./bin
	g++ $(opt) $(rshellsrc) -o $(rshellout)

rshell:
	mkdir ./bin
	g++ $(opt) $(rshellsrc) -o $(rshellout)

ls:
	mkdir ./bin
	g++ $(opt) $(lssrc) -o $(lsout)
clean:
	$(rmb)
