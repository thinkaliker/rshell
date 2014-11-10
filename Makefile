bindir=./bin
opt=-Wall -Werror -ansi -pedantic
dbgopt=$(opt) -g
out=-o $(bindir)
src=./src
rmb=rm -rf $(bindir)
chkdir=if [ -d "$(bindir)" ]; then  $(rmb); fi

.PHONY: all
all: dir rshell ls cp

dir:
	$(chkdir)
	mkdir $(bindir)
rshell:
	g++ $(opt) $(src)/main.cpp $(out)/rshell
ls:
	g++ $(opt) $(src)/ls.cpp $(out)/ls
cp:
	g++ $(opt) $(src)/cp.cpp $(out)/cp
debug:
	$(chkdir)
	mkdir $(bindir)
	g++ $(dbgopt) $(src)/main.cpp $(out)/rshell
	g++ $(dbgopt) $(src)/cp.cpp $(out)/cp
	g++ $(dbgopt) $(src)/ls.cpp $(out)/ls
clean:
	$(rmb)
