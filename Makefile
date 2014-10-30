bindir=./bin
opt=-Wall -Werror -ansi -pedantic
rshellout=-o ./bin/rshell
rshellsrc=./src/main.cpp
lsout=-o ./bin/ls
lssrc=./src/ls.cpp
rmb=rm -rf ./bin

all:
	if [ -d "$(bindir)" ]; then  $(rmb); fi
	mkdir ./bin
	g++ $(opt) $(rshellsrc) $(rshellout)
	g++ $(opt) $(lssrc) $(lsout)

rshell:
	mkdir ./bin
	g++ $(opt) $(rshellsrc) $(rshellout)

ls:
	mkdir ./bin
	g++ $(opt) $(lssrc) $(lsout)
clean:
	$(rmb)
