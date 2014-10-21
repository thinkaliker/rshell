#include <errno.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

string shell_prompt(); //prototype for prompt function

int main (int argc, char** argv)
{
	while(true)
	{
		string input;
		vector<string> inVector;

		input = shell_prompt();
		if (input != "exit" || input != "Exit")
		{
			char_separator<char> sep(";|&");
			tokenizer< char_separator<char> > tokens(input, sep);
			string t;
			BOOST_FOREACH(t, tokens);
			{
				inVector.push_back(t);
				cout << t << endl;
			}
	
			//tokenizer<> tokens(shell_prompt());
		}
		else
		{
			cout << "Goodbye." << endl;
		}
		
	}
	/*
	int pid = fork();
	if (pid == 0)
	{	
		int error = execvp(argv[0], argv);
		if (error == -1)
		{
			perror("execvp"); //throw an error
			exit(1);
		}
		else
		{
			if (argv[0] != "exit")
			{
				//execute
			}
			exit(0);
		}
	}
	else
	{
		wait(NULL);
	}
	*/	
	return 0;
}

string shell_prompt()
{
	string in;
	//TODO - error checking for getlogin and gethostname
	//implement later
	/*
	char name[256];
	int maxlen = 64;
	if (!gethostname(name, maxlen))
	{
		string strname(name);
		cout << getlogin() << "@" << name  << "$ "; //custom prompt with hostname and login name
		cin >> in;
	}
	else
	{
		perror("gethostname"); //throw error if not found
	}
	*/
	cout << "rshell$ ";
	cin >> in;
	return in;
}
