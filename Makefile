bindir=./bin
opt=-Wall -Werror -ansi -pedantic
rshellout=-o ./bin/rshell
rshellsrc=./src/main.cpp
cpout =-o ./bin/cp
cpsrc =./src/cp.cpp
lsout=-o ./bin/ls
lssrc=./src/ls.cpp
rmb=rm -rf $(bindir)
chkdir=if [ -d "$(bindir)" ]; then  $(rmb); fi


all:
	$(chkdir)
	mkdir $(bindir)
	g++ $(opt) $(rshellsrc) $(rshellout)
	g++ $(opt) $(lssrc) $(lsout)
	g++ $(opt) $(cpsrc) $(cpout)

rshell:
	$(chkdir)
	mkdir $(bindir)
	g++ $(opt) $(rshellsrc) $(rshellout)

ls:
	$(chkdir)
	mkdir $(bindir)
	g++ $(opt) $(lssrc) $(lsout)
cp:
	$(chkdir)
	mkdir $(bindir)
	g++ $(opt) $(cpsrc) $(cpout)
clean:
	$(rmb)
