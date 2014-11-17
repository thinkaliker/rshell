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
#include <sys/utsname.h>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

string shell_prompt(); //prototype for prompt function
int cmd_interpreter(string);//, char**); //prototype for command interpreter

int main (int argc, char** argv)
{
	while(true)
	{	
		vector<string> inVector;
		string input;
		input = shell_prompt();
		//while (inVector.back() != "")
		if (input == "exit")
		{
			cout << "Exiting rshell." << endl;
			exit(0);
		}
		else
		{
			char_separator<char> sep(";", "|&#<>");
			string t;
			tokenizer< char_separator<char> > tokens(input, sep);
			BOOST_FOREACH(t, tokens)
			{
				//TODO do different things depending on delimiters in vector
				inVector.push_back(t); 
			} //end BOOST_FOREACH
			
			bool comment_sentinel = true;
			bool pipe_sentinel = false;

			for (unsigned i = 0; i < inVector.size(); i++) //go through vector of commands - looking for comments, pipes, redirections
			{
				if(comment_sentinel)	//if a comment sign is not found, execute
				{
					string in = inVector.at(i);
					if (in.at(0) == '#')
					{
						comment_sentinel = false;
						continue;
					}

					if (in.at(0) == '&')
					{
						//TODO: remove later and fix
						continue;
					}

					if (in.at(0) == '|')
					{
						if (inVector.at(i + 1).at(0) == '|') //likely to go out of range if at end of command
						{
							pipe_sentinel = true;
							continue;
						}
						if (pipe_senitnel)
						{
							//TODO: remove later and fix
							continue;
						}
						
					}
					
										
					cmd_interpreter(in);
				}
				
				
				

				//TODO: this is for connectors
				// check if the current in.at(0) character equals a connector
				// if it does, check if the next one equals a connector
				// if it does, run both commands
				// check return values of both commands
				// ????
				// profit
			}//endfor
		}//endif
	}//endwhile

	return 0;
}

int cmd_interpreter(string input)//, char** argv)
{
	//parse command to seperate command and parameters

	//int len = input.length();
	
	vector<string> invector;
	string t;
	char_separator<char> sep(" ");
	tokenizer< char_separator<char> > tokens(input, sep);
	//int i = 0;
	BOOST_FOREACH(t, tokens)		//tokenize input string with flags to seperate items
	{
		invector.push_back(t);
	}
	unsigned len = invector.size();	

	const char** cinput = new const char*[len+2];
	const char* program = invector.at(0).c_str();

	cinput[0] = program;

	for(unsigned i = 1; i < 1 + len; i++)
	{
		cinput[i] = invector[i].c_str();
	}
	cinput[len] = '\0';

//	strcpy(argv[0], input.c_str());
//	strcat(argv[0], "\0");
	//cerr << argv[0] << endl;
	int pid = fork();
	if(pid == 0)
	{
//		int error = execvp(argv[0], argv);
		if (execvp(program, (char**)cinput) == -1)
		{
			perror("execvp"); // throw an error
			exit(1);
			//return error;
		}
		//else
		//{
		//	execvp(argv[0], argv);
		//}
	}
	else
	{
		//parent wait
		if (waitpid(-1, NULL, 0) == -1)
		{
			perror("waitpid");
			exit(1);
		}
	}

	return 0;
	
} 

string shell_prompt()
{
	string in;
	//TODO - error checking for getlogin and gethostname
	//implement later
//	struct utsname name;
//	errno = 0;

//	uname(&name)
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
