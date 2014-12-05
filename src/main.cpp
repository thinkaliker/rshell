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
#include <algorithm>
#include <functional>
#include <dirent.h>
#include <signal.h>
#include <csignal>
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
int cmd_interpreter(string);//, bool, bool); //prototype for command interpreter
void input_redir(vector<string>); //prototype for input redirection function
int input_helper(string, string);
void output_redir(vector<string>); //prototype for output redirection function
int output_helper(string, string);
const char* convert(const string);
bool read_dir(const char*, const char*);
void change_dir(string);
void signal_handler(int);

int main (int argc, char** argv)
{
	if (signal(SIGINT, signal_handler) == SIG_ERR)
	{
		perror("signal");
		exit(1);
	}
	while(true)
	{	
		vector<string> inVector;
		string input;
		input = shell_prompt();
		//while (inVector.back() != "")
		if (input == "exit")
		{
			//exits
			cout << "Exiting rshell." << endl;
			exit(0);
		}
		else
		{
			//take in input
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
			bool cd_sentinel = false;

			for (unsigned i = 0; i < inVector.size(); i++) //go through vector of commands - looking for comments
			{
				if((comment_sentinel)&&(!cd_sentinel))	//if a comment sign is not found, execute
				{
					string in = inVector.at(i);
	//		cerr << "[ " << in << " ]" << endl;
					if (in.at(0) == '#')
					{
						comment_sentinel = false;
					}
					else
					{
//						if((in == "cd")||!cd_sentinel)
						string cd = "cd";
//			cerr << "compare: " << in.compare(0,2, cd) << endl;
						if(in.compare(0, 2, cd) == 0 )
						{
							//change directory
							change_dir(in);
							cd_sentinel = true;
							break;
						}
						else if (cd_sentinel)
						{
							break;
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
									if (inVector.at(k + 1).at(0) == '|') //likely to go out of range if at end of command
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
									 //nothing
									continue;
								}
							}
							cmd_interpreter(in);//, true, false);
						}
					}
				}	//		cerr << "test" << endl;

				
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

void signal_handler(int signum)
{
		if(signum == SIGINT)
		{
			//quit signal called
			return;
		}
		else if (signum == SIGSTOP)
		{
			//background
			return;
		}

}

void change_dir(string in)
{
	if(in.length() > 3)
	{
		vector<string> input;
		string t;
		char_separator<char> sep(" ");
		tokenizer< char_separator<char> > tokens(in, sep);
		BOOST_FOREACH(t, tokens)		//tokenize input string with flags to seperate items
		{
			input.push_back(t);
		}
	//	cerr << input.at(0) << " to " << input.at(1) << endl;
		//need chdir

		if (chdir(input.at(1).c_str()) == -1)
		{
			perror("chdir");
		}
	}
	else
	{
		cerr << "Error: directory name not provided" << endl;
	}
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
					cmd_interpreter(one);//, false, false);
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
		if (pid == -1)
		{
			perror("fork");
			exit(1);
		}
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
	//handles all output redirectionThen pass it to your function via the .data() member of vector:
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
//cerr << one << " " << two << endl;
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
					cmd_interpreter(one);//, false, false);
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
		if (pid == -1)
		{
			perror("fork");
			exit(1);
		}
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

int cmd_interpreter(string input)//, bool fg, bool bg)//, char** argv)
{
	//parse command to seperate command and parameters

	bool relative = false;
	string cd = "./";
	if(input.compare(0, 2, cd) == 0)
	{
		input.erase(0, 2);
		relative = true;
	}

	vector<string> invector;
	string t;
	vector<char*> cinput;
	cinput.reserve(16);
	char_separator<char> sep(" ");
	tokenizer< char_separator<char> > tokens(input, sep);
	BOOST_FOREACH(t, tokens)		//tokenize input string with flags to seperate items
	{
		char* stuff = new char[t.size()+1];
		strcpy(stuff, t.c_str());
		cinput.push_back(stuff);
//		cerr << "stuff: " << stuff << endl;
	}
//	cinput.push_back(input.c_str());
	char* zero = '\0';
	cinput.push_back(zero);	//put the null terminating charater in back

//cerr << "cinput size: " << cinput.size() << endl;

	char*  envstr = getenv("PATH");
	if (envstr == NULL)
	{
		perror("getenv");
		exit(0);
	}
	string env (envstr);
	vector<string> paths;
	split(paths, env, is_any_of(":"));

	if(relative)
	{
		string d = ".";
		vector<string>::iterator it;
		it = paths.begin();
		it = paths.insert(it, d);
	}

	//iterate through entire path vector and check if file exists
	//
//	for (unsigned i = 0; i < cinput.size(); i++)
//	{
//		cout << "CINPUT: " << cinput[i] << endl;
//	}
//	cout << flush;
	
	bool find_flag = false;
	char* cmd = cinput[0];
	for(unsigned i = 0; i < paths.size(); i++)
	{
//		char* cmd = cinput[0];
		paths.at(i) += "/";
//		cerr << "pathstuff: " << paths.at(i) << " | " << cmd  << " |" << endl;

		if(read_dir(paths.at(i).c_str(), cmd) && (!find_flag))
		{
			find_flag = true;
			string input = paths.at(i)+string(cmd);
//			paths.at(i) += string(cmd).c_str();
//			paths.at(i) += "\0";
//	cerr << "pathappend: " << input << endl;

			int pid = fork();
			if(pid == 0)
			{
	/*			if(setpgid(0,0) == -1)
				{
					perror("setpgid");
					exit(1);
				}
				else if (fg)
				{
					if(tcsetpgrp(0,0) == -1)
					{
						perror("tcsetpgrp");
						exit(1);
					}
				}
				else
				{
					;
				}
			//	signal(SIGTTOU, SIG_DFL);
			//	signal(SIGTTIN, SIG_DFL);
				*/
//		cerr << "exec: " << paths.at(i).c_str()[0] << " | " << &cinput[0] << endl;
				if ((execv(input.c_str(), &cinput.front())) == -1) //&cinput.front())) == -1)
				{
					perror("execv"); // throw an error
					exit(1);
				}
				else
				{
					return 1;
				}
			}
			else
			{
				if (pid == -1)
				{
					perror("fork");
					exit(1);
				}
			/*	if(fg)
				{
					if(tcsetpgrp(0,0) == -1)
					{
						perror("tcsetpgrp");
						exit(1);
					}

				//	signal(SIGTTOU, SIG_DFL);
				//	signal(SIGTTIN, SIG_DFL);
					*/
									//parent wait
					if (waitpid(0, NULL, WUNTRACED) == -1)
					{
						perror("waitpid");
						exit(1);
					}
/*
					if(tcsetpgrp(0,0) == -1)
					{
						perror("tcsetpgrp");
						exit(1);
					}

				}
				if(bg)
				{
			//		signal(SIGTTIN, SIG_DFL);
			//		signal(SIGTTOU, SIG_DFL);
				}
				*/
			}
		}
	}
	if (!find_flag)
	{
		cout << "Error: could not find " << cinput.at(0) << endl;
	}

	return 0;
} 

string shell_prompt()
{
	string in;
	char name[256];
	size_t maxlen = 64;
	if (gethostname(name, maxlen) != -1)
	{
		errno = 0;
		char* strlogin = getlogin();
		if (strlogin == NULL)
		{
			perror("getlogin");
			exit(1);
		}
		errno = 0;
		char* strcwd = get_current_dir_name();
		if (errno != 0)
		{
			perror("getcwd");
			exit(1);
		}
		else
		{
			cout << strlogin << "@" << name << ":" << strcwd << "$ "; //custom prompt with hostname and login name
		}
	}
	else
	{
		perror("gethostname"); //throw error if not found
	}

//	cout << "rshell$ ";
	getline(cin, in);
	cin.clear();
	return in;
}

const char* convert(const string &str)
{
	return str.c_str();
}

bool read_dir(const char* dirName, const char* name)
{
	errno = 0;
	DIR *dirp = opendir(dirName);
	if(errno == 0)
	{
//		cerr << "searching..." << name << " in " << dirName << endl;
		while (true)
		{
			dirent *dp;
			if ((dp = readdir(dirp)) != NULL)
			{
				if(strcmp(dp->d_name, name) == 0)
				{
					if(closedir(dirp) != 0)
					{
						perror("closedir");
					}
					else
					{
						return true;
					}
				}
			}
			else
			{
				if (closedir(dirp) != 0)
				{
					perror("closedir");
				}
				return false;
			}
		}
		return false;
	}
	else
	{
		return false;
		perror("opendir");
//		return false;
	}
}
