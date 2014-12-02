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
const char* convert(const string);
bool read_dir(const char*, const char*);

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

			for (unsigned i = 0; i < inVector.size(); i++) //go through vector of commands - looking for comments
			{
				if(comment_sentinel)	//if a comment sign is not found, execute
				{
					string in = inVector.at(i);
//			cerr << "[ " << in << " ]" << endl;
					if (in.at(0) == '#')
					{
						comment_sentinel = false;
					}

					else
					{
						if(inVector.at(i) == "cd")
						{
							//change directory

							continue;
						}
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
	vector<char*> cinput;
	char_separator<char> sep(" ");
	tokenizer< char_separator<char> > tokens(input, sep);
	BOOST_FOREACH(t, tokens)		//tokenize input string with flags to seperate items
	{
		char* stuff = const_cast<char*>(string(t).c_str());
		cinput.push_back(stuff);
	}
	cinput.push_back(NULL);	//put the null terminating charater in back
	//transform(invector.begin(), invector.end(), back_inserter(cinput), convert);
		
//	for (unsigned i = 0; i < len; i++)
//	{
//		cinput.push_back(invector.at(i).c_str());
//	}
//	const char** cinput = new const char*[len+2];
//	const char* program = invector.at(0).c_str();

//	cinput[0] = program;

//	for(unsigned i = 1; i < 1 + len; i++)
//	{
//		cinput[i] = invector[i].c_str();
//	}
//	cinput[len] = '\0';

	char*  envstr = getenv("PATH");
	if (envstr == NULL)
	{
		perror("getenv");
		exit(0);
	}
	string env (envstr);
	vector<string> paths;
	split(paths, env, is_any_of(":"));

	paths.push_back(".");	//current directory last	
	//iterate through entire path vector and check if file exists
	
	bool find_flag = false;
	for(unsigned i = 0; i < paths.size(); i++)
	{
		paths.at(i) += "/";
		if(read_dir(paths.at(i).c_str(), cinput.at(0)) && !find_flag)
		{
			paths.at(i) += cinput.at(0);
	//		cerr << "test" << endl;
			find_flag = true;
			int pid = fork();
			if(pid == 0)
			{
				if ((execv(paths.at(i).c_str(), &cinput.front())) == -1)
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

				//parent wait
				if (waitpid(-1, NULL, 0) == -1)
				{
					perror("waitpid");
					exit(1);
				}
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
		char* strlogin = getlogin();
		if (strlogin == NULL)
		{
			perror("getlogin");
			exit(1);
		}
	
		cout << strlogin << "@" << name  << "$ "; //custom prompt with hostname and login name
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
			errno = 0;
			dirent *dp;
			if ((dp = readdir(dirp)) != NULL)
			{
				if(strcmp(dp->d_name, name) == 0)
				{
					if(closedir(dirp) != 0)
					{
						perror("closedir");
						return false;
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
					return false;
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
/*


	if ((dirp == 0))
	{
		perror("opendir");
	}
	else
	{
		dirent *direntp;
		while (true)
		{
			errno = 0;
			if ((direntp = readdir(dirp)) != 0)
			{
				return true;
				continue;
			}
			if(errno != 0)
			{
				perror("readdir");
			}
			break;
		}
		if (closedir(dirp) != 0)
		{
			perror("closedir");
		}
	}
	return false;
	*/
}
