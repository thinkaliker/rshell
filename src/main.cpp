#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main (int argc, char** argv)
{
	int pid = fork();
	if (pid == 0)
	{	
		int error = execvp(argv[0], argv);
		if (error == -1)
		{
			perror("execvp");
		}
	}
	else
	{
		wait(NULL);
	}
		
	return 0;
}
