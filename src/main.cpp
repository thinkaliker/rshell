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
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

string shell_prompt(); //prototype for prompt function
char[] cmd_interpreter(vector<string>); //prototype for command interpreter

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
			cout << "Exiting rshell." << endl;
			exit(0);
		}
		else
		{
			char_separator<char> sep(";|&", "|&");
			string t;
			tokenizer< char_separator<char> > tokens(input, sep);
			BOOST_FOREACH(t, tokens);
			{
				//TODO do different things depending on delimiters in vector
				inVector.push_back(t);
			}
			cmd_interpreter(inVector);
		}
	}

	return 0;
}

char[] cmd_interpreter(vector<string> input)
{
//	unsigned size = input.size();
	string cmd = input.back();
 	string file;
	int len = cmd.length();
//	char* param;

	vector<string> strs;

//	cout << "last " << input.back() << endl;
//	for (unsigned i = 0; i < size ; i++)
//		cout << "test " << input.at(i) << endl;

	//parse command to seperate command and parameters
	
	split(strs, cmd, is_any_of(" "));
//	file = strs.front();
	
//	for (unsigned i = 1; i < strs.size(); i++)
//	{
//		param.append(strs.at(i));
//	}
//	param.append('\0'); //null termination
//	const char* filec = file.c_str();
//	const char* paramc = param.c_str();

	len = cmd.length();
	file = strs.front();
	cout << "file:" << file << endl;
	
	char* const paramc = new char[len+1];
//	char* const charc = new  [file.length()];
	for (int i = 0; i != len; i++)
	{
		if (i == len)
		{
			paramc[i] = '\0';
		}
		paramc[i] =  strs.at(i).c_str();	
	}

}

int execute(char* const cmd)
{	
	//fork execution to it's own process
	int pid = fork();
	if(pid == 0)
	{
		int error = execvp(cmd[0], cmd);
		if (error == -1)
		{
			perror("execvp"); // throw an error
			exit(1);
			return error;
		}
		else
		{
			execvp(cmd[0], cmd);
		}
	}
	else
	{
		//parent wait
		waitpid(-1, NULL, 0);
	}

	return;
	
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
