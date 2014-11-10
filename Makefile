bindir=./bin
opt=-Wall -Werror -ansi -pedantic
dbgopt=-Wall -Werror -ansi -pedantic -g
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
	g++ $(opt) $(cpsrc) $(cpout)
	g++ $(opt) $(lssrc) $(lsout)

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
debug:
	$(chkdir)
	mkdir $(bindir)
	g++ $(dbgopt) $(rshellsrc) $(rshellout)
	g++ $(dbgopt) $(cpsrc) $(cpout)
	g++ $(dbgopt) $(lssrc) $(lssrc)
clean:
	$(rmb)
