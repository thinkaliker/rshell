#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <clocale>
#include <ctime>
using namespace std;

#define FLAG_a 1
#define FLAG_l 2
#define FLAG_R 4

//bool is_directory(char* path);
void check_modifiers(const char* path);
void check_dir(const char* path, vector<string>&);
int find_index(int argc, char** argv);

int main(int argc, char** argv)
{
	int flags = 0;
	vector<string> files;
	
	setlocale(LC_ALL, "en_US.UTF-8");  //set locale for time format

	//set flags by iterating through entire command line arguments
	while (true)
	{
        	int c = getopt (argc, argv, "alR");   //sorts flags to front but non flags to back
		if (c == -1)
		{
			break; //reached end of argv
		}
        	if (c == 'a')
			flags |= FLAG_a;
		if (c == 'l')
			flags |= FLAG_l;
		if (c == 'R')
			flags |= FLAG_R;
     	}
	int index = find_index (argc, argv);

//TODO: check if directory name is provided, otherwise 

//TODO: implement whatever this is which has something to do with hidden files
	if (flags & FLAG_a)
	{
//		cout << "dbg index" << index << endl;
		const char *dirName = ".";
		if (argv[index] == 0)
		{
			dirName = ".";
		}
		else
		{
			dirName = argv[index];
		}
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
		const char *dirName = ".";
		if (argv[index] == 0)
		{
			dirName = ".";
		}
		else
		{
			dirName = argv[index];
		}
		check_modifiers(dirName);

		//additional info like user, machine, additional stuff
	}
	

//TODO: implement the recursive function...ew, recursion

//TODO: create a case where a file is passed in as a parameter

//TODO: create a case where a directory is passed in without any flags

//TODO: create a default case where no parameters are given


	if ((argv[index-1] == 0))
	{
		const char *dirName = ".";
		check_dir(dirName, files);
		sort(files.begin(), files.end());
		for (unsigned i = 0; i < files.size(); i++)
		{
			
			if (files.at(i).at(0) != '.')
			{
				//cout << left << setw(11) << files.at(i) ;
				cout << setw(files.at(i).size()) << files.at(i);
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


void check_modifiers(const char* name)
{
	struct stat statbuf;
	
	if(stat(name, &statbuf) == 0)
	{

		//directory/symbolic link
		if(S_ISDIR(statbuf.st_mode))
			cout << "d";
		else if (S_ISLNK(statbuf.st_mode))
			cout << "l";
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
		cout << flush;	
		cout << setw(4) << left << " " << statbuf.st_nlink << " "; //number of symbolic links

		errno = 0;
		struct passwd *p;
		p = getpwuid(statbuf.st_uid);
		if((errno == 0) && (p != NULL))
		{
			cout << setw(6) << left << p->pw_name << " " << flush;  //print out username based on UID
		}
		else
		{
			perror("getpwuid");
			exit(1);
		}

		errno = 0;
		struct group *g = getgrgid(statbuf.st_gid);
		if ((errno != 0))
		{
			perror("getpwuid");
			exit(1);
		}
		else
		{
			cout << setw(6) << left << g->gr_name << flush;  //print out group name based on GID
		}

		cout << setw(6) << right << statbuf.st_size << "  "; //print out size in bytes

		struct tm *localTime;
		time_t t = statbuf.st_mtime;
		if((localTime = localtime(&t)) != NULL)
		{
			char timestr[100];
			if((strftime(timestr, sizeof(timestr), "%b %e %H:%M", localTime)) != 0)
			{
				cout << " " << timestr << flush;
			}
			else	
			{
				perror("strftime");
				exit(1);
			}

		}
		else
		{
			perror("localtime");
			exit(1);
		}

		cout << "  " << name << flush;  //location

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

int find_index(int argc, char** argv)
{
	int index = 1;
	for (int i = 1; i < argc; ++i)
	{
 		if (argv[i][0] == '-')
		{
			index++;
		}
	}
	return index;
}
