#include <errno.h>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <pwd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

string shell_prompt(); //prototype for prompt function
int cmd_interpreter(string); //prototype for command interpreter
void input_redir(vector<string>); //prototype for input redirection function
int input_helper(string, string);
void output_redir(vector<string>); //prototype for output redirection function
int output_helper(string, string);

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

			for (unsigned i = 0; i < inVector.size(); i++) //go through vector of commands - looking for comments
			{
				if(comment_sentinel)	//if a comment sign is not found, execute
				{
					string in = inVector.at(i);
			cerr << "[ " << in << " ]" << endl;
					if (in.at(0) == '#')
					{
						comment_sentinel = false;
					}

					else
					{
						for (unsigned k = 0; k < inVector.size(); k++)
						{

							if (inVector.at(k).at(0) == '&')
							{
								//TODO: remove later and fix
							}

							else if (inVector.at(k).at(0) == '|')
							{
								if (inVector.at(i + 1).at(0) == '|') //likely to go out of range if at end of command
								{
									pipe_sentinel = true;
								}
								if (pipe_sentinel)
								{
									//TODO: remove later and fix
								}
							
							}
					
							else if (inVector.at(k).at(0) == '<')
							{
								//input redirection
					//	cerr << "we indir i hope" << endl;
								input_redir(inVector);
								 //input_redir handles
								 comment_sentinel = false; //force a skip
								 break;
							}
						
							else if (inVector.at(k).at(0) == '>')
							{
								//output redirection
					//	cerr << "we outdir i hope" << endl;
								output_redir(inVector);
								 //output_redir function handles this
								 comment_sentinel = false; //force a skip
								 break;
								 
							}
							else
							{					
								; //nothing
								continue;
							}
						}
						cmd_interpreter(in);
					}
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

void input_redir(vector<string> input)
{
	//handles all of input redirection
	for (unsigned i = 0; i < input.size(); i++)
	{
		if (input.at(i).at(0) == '<')
		{
//	cerr << "we input now" << endl;
			input_helper(input.at(i-1), input.at(i+1));
		}
	}
}

int input_helper(string one, string two)
{
	int pid = fork();
	if (pid == 0)
	{
		//child
		//open close dup
		if (open(two.c_str(), O_RDONLY) != -1)
		{
			if(close(0) != -1) //stdin
			{
				if(dup(0) != -1)
				{
		//cerr << one << endl;
					cmd_interpreter(one);
					return 1;
				}
				else
				{
					perror("dup");
					exit(1);
				}
			}
			else
			{
				perror("close");
				exit(1);
			}
		}
		else
		{
			perror("open");
			exit(1);
		}
		if (close(0) == -1)
		{
			perror("close");
			exit(1);
		}
	}
	else
	{
		//parent
//		close(0);
		if (waitpid(-1, NULL, 0) == -1)
		{
			perror("waitpid");
			exit(1);
		}
		if(close(0) == -1)
		{
			perror("close");
			exit(1);
		}
		return 1;
	}
	
	return 0;
}

void output_redir(vector<string> input)
{
	//handles all output redirection
	for (unsigned i = 0; i < input.size(); i++)
	{
		//iterate through vector and finds redirection		
		if (input.at(i).at(0) == '>')
		{
//		cerr << "we output now" << endl;
			output_helper(input.at(i-1), input.at(i+1));
		}
	}
}

int output_helper(string one, string two)
{
cerr << one << " " << two << endl;
	int pid = fork();
	if (pid == 0)
	{
		//child
		//open close dup
		if (open(two.c_str(), O_WRONLY | O_CREAT) != -1)
		{
			if(close(1) != -1) //stdin
			{
				if(dup(1) != -1)
				{
					cmd_interpreter(one);
					return 1;
				}
				else
				{
					perror("dup");
					exit(1);
				}
			}
			else
			{
				perror("close");
				exit(1);
			}
		}
		else
		{
			perror("open");
			exit(1);
		}
		if (close(1) == -1)
		{
			perror("close");
			exit(1);
		}
	}
	else
	{
		//parent
//		close(1);
		if (waitpid(-1, NULL, 0) == -1)
		{
			perror("waitpid");
			exit(1);
		}
		if (close(1) == -1)
		{
			perror("close");
			exit(1);
		}
		return 1;
	}
	
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

//	int pipefd[
	int pid = fork();
	if(pid == 0)
	{
		if (execvp(program, (char**)cinput) == -1)
		{
			perror("execvp"); // throw an error
			exit(1);
		}
		else
		{
			return 1;
		}
	}
	else
	{
		//append stuff here

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
