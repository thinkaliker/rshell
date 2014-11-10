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
void check_modifiers(const char* path);
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
		if ((argv[i][0] == '\\') || ((argv[i][0] >= '0') && (argv[i][0] <= 'z')))
		{
			//this is a directory or a file
			;
		}
	}
	
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
		
		const char *dirName = "/bin";
		check_modifiers(dirName);
		

		//additional info like user, machine, additional stuff
	}
	

//TODO: implement the recursive function...ew, recursion

//TODO: create a case where a file is passed in as a parameter

//TODO: create a case where a directory is passed in without any flags

//TODO: create a default case where no parameters are given


	if ((argv[1] == 0))
	{
		const char *dirName = ".";
		check_dir(dirName, files);
		sort(files.begin(), files.end());
		for (unsigned i = 0; i < files.size(); i++)
		{
			
			if (files.at(i).at(0) != '.')
			{
				//cout << left << setw(11) << files.at(i) ;
				cout << files.at(i) << "  ";
			}
			/*
			if (((i % 10) == 0) && (i != 0))
			{
				cout << endl;
			}*/
		}
		cout << endl;
	}


	return 0;
}

bool is_directory(char* path)
{
	//function to check if a path is a directory or not
	return true;
}


void check_modifiers(const char* dirName)
{
	struct stat statbuf;
	
	if(stat(dirName, &statbuf) == 0)
	{

		//directory
		if(S_ISDIR(statbuf.st_mode))
			cout << "d";
		else 
			cout << "-";
		//user cluster
		if(statbuf.st_mode & S_IRUSR)
			cout << "r";
		else 
			cout << "-";
		if(statbuf.st_mode & S_IWUSR)
			cout << "w";
		else 
			cout << "-";
		if(statbuf.st_mode & S_IXUSR)
			cout << "x";
		else 
			cout << "-";
		//group cluster
		if(statbuf.st_mode & S_IRGRP)
			cout << "r";
		else 
			cout << "-";
		if(statbuf.st_mode & S_IWGRP)
			cout << "w";
		else 
			cout << "-";
		if(statbuf.st_mode & S_IXGRP)
			cout << "x";
		else 
			cout << "-";
		//other cluster
		if(statbuf.st_mode & S_IROTH)
			cout << "r";
		else 
			cout << "-";
		if(statbuf.st_mode & S_IWOTH)
			cout << "w";
		else 
			cout << "-";
		if(statbuf.st_mode & S_IXOTH)
			cout << "x";
		else 
			cout << "-";

		//more stuff			

		cout << endl;
	}
	else
	{
		perror("stat");
		exit(1);
	}
}


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
