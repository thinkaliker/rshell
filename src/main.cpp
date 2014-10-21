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
void cmd_interpreter(vector<string>); //prototype for command interpreter

int main (int argc, char** argv)
{
	vector<string> inVector;
	string input;

	while(true)
	{	
		input = shell_prompt();
		//while (inVector.back() != "")
		if (input == "exit")
		{
			cout << "Exiting." << endl;
			exit(0);
		}
		else
		{
			char_separator<char> sep(";|&");
			string t;
			tokenizer< char_separator<char> > tokens(input, sep);
			BOOST_FOREACH(t, tokens);
			{
				inVector.push_back(t);
//				cout << t << endl;
			}
		}
		cmd_interpreter(inVector);
	}

	return 0;
}

void cmd_interpreter(vector<string> input)
{
	unsigned size = input.size();

//	if (!input.empty())
	{
	for (unsigned i = 0; i < size  ; i++)
		cout << "test " << input.at(i) << endl;
	}
	return;

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
	getline(cin, in);
	cin.clear();
	return in;
}
