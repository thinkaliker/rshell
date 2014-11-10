#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <iomanip>
#include <algorithm>
using namespace std;

#define FLAG_a 1
#define FLAG_l 2
#define FLAG_R 4

bool is_directory(char* path);
//void check_mod(stat &statbuf);
void check_dir(const char* path, vector<string>&);

int main(int argc, char** argv)
{
	int flags = 0;
	
	//set flags by iterating through entire command line arguments
	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			for(int j = 1; argv[i][j] != 0; j++)
			{
				if (argv[i][j] == 'a')
					flags |= FLAG_a;
				if (argv[i][j] == 'l')
					flags |= FLAG_l;
				if (argv[i][j] == 'R')
					flags |= FLAG_R;
			}
		}
	}
	
	struct stat statbuf;
	vector<string> files;

//TODO: check if directory name is provided, otherwise 

//TODO: implement whatever this is which has something to do with hidden files
	if (flags & FLAG_a)
	{	
		const char *dirName = ".";
		check_dir(dirName, files);
		sort(files.begin(), files.end());
		for (unsigned i = 0; i < files.size(); i++)
		{
			cout << files[i] << "  ";
		}
		cout << endl;

	}
//TODO: implement the detailed list case with the drwx items using statbuf and .st_mode
	if (flags & FLAG_l)
	{
		//need more code here about stuff like passing in the current directory

		stat("bin", &statbuf); //change bin to take in input later
		

		//additional info like user, machine, additional stuff
	}
	

//TODO: implement the recursive function...ew, recursion

//TODO: create a case where a file is passed in as a parameter

//TODO: create a case where a directory is passed in without any flags

//TODO: create a default case where no parameters are given


	if (false)//CHANGE THIS PLS
	{
		const char *dirName = ".";
		check_dir(dirName, files);
		for (unsigned i = 0; i < files.size(); i++)
		{
			if (files.at(i).at(0) != '.')
			{
				cout << left << setw(10) << files.at(i) ;
			}
			if ((i % 8) == 0)
			{
				cout << endl;
			}
		}
	}


	return 0;
}

bool is_directory(char* path)
{
	//function to check if a path is a directory or not
	return true;
}

//void check_mod(stat &statbuf)
/*
{

	if(S_ISDIR(statbuf.st_mode))
		cout << "d";
	else cout << "-";
	if(statbuf.st_mode & S_IRUSR)
		cout << "r";
	else cout << "-";
	if(statbuf.st_mode & S_IWUSR)
		cout << "w";
	else cout << "-";
	if(statbuf.st_mode & S_IXUSR)
		cout << "x";
	else cout << "-";
	

}
*/

void check_dir(const char* dirName, vector<string>& files)
{
	DIR *dirp = opendir(dirName);
	if ((dirp == 0))
	{
		perror("opendir");
		exit(1);
	}
	else
	{
		errno = 0;
		dirent *direntp;
		while (true)
		{
			if ((direntp = readdir(dirp)) != 0)
			{
//				cout << direntp->d_name << endl;
				files.push_back(direntp->d_name);
				continue;
			}
			if(errno != 0)
			{
			perror("readdir");
			exit(1);
			}
			break;
		}
		if (closedir(dirp) != 0)
		{
			perror("closedir");
			exit(1);
		}
	}
	
}
