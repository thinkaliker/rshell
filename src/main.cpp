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
			cout << "Exiting rshell." << endl;
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
//	unsigned size = input.size();
	string cmd = input.back();
//	string file;
//	string param;
	int len;
//	vector<string> strs;

//	cout << "last " << input.back() << endl;
//	for (unsigned i = 0; i < size ; i++)
//		cout << "test " << input.at(i) << endl;

	//parse command to seperate command and parameters
	
//	split(strs, cmd, is_any_of(" "));
//	file = strs.front();
	
//	for (unsigned i = 1; i < strs.size(); i++)
//	{
//		param.append(strs.at(i));
//	}
//	param.append('\0'); //null termination
//	const char* filec = file.c_str();
//	const char* paramc = param.c_str();

	len = cmd.length();
	char* const paramc = new char[len + 1];
	for (int i = 0; i < len; i++)
	{
		paramc[i] = cmd[i];
	}
	paramc[len] = '\0';
	
	char* const argv[] = {paramc};
	
//	char*const param[] = {
//
//	char filec[1] = file;
//	char paramc[size+1] = param;

//	strcpy(filec, file.c_str());
	
	//fork execution to it's own process
	int pid = fork();
	if(pid == 0)
	{
		int error = execvp(argv[0], argv);
		if (error == -1)
		{
			perror("execvp"); // throw an error
			exit(1);
		}
		else
		{
			execvp(argv[0], argv);
		}
	}
	else
	{
		//parent wait
		waitpid(-1, NULL, 0);
	}
	delete [] paramc;

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
