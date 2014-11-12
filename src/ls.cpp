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
void check_modifiers(const char* path, const char* name);
void check_dir(const char* path, vector<string>&);
int find_index(int argc, char** argv);
void print_files(vector<string>, int start, bool hidden);

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
			dirName = "./";
		}
		else
		{
			dirName = argv[index];
		}
		check_dir(dirName, files);
		sort(files.begin(), files.end());
		print_files(files, 0, true);
		cout << endl;
		
	}
//TODO: implement the detailed list case with the drwx items using statbuf and .st_mode
	if (flags & FLAG_l)
	{
		//need more code here about stuff like passing in the current directory
		int relative = 0;
		const char *dirName = "./";
		if (argv[index] == 0)
		{
			dirName = "./";
			cout << "nname: " << dirName << endl;
			relative = 1;
		}
		else if (argv[index][0] != '/')
		{
			dirName = argv[index];
			cout << "rname: " << dirName << endl;
			relative = 2;
		}
		else
		{
			dirName = argv[index];
			cout << "name: " << dirName << endl;
			relative = false;
		}
//		cout << "dir: " << dirName << endl;
		check_dir(dirName, files);
		sort(files.begin(), files.end());
		for (unsigned i = 0; i < files.size(); i++)
		{
			if (relative == 1)
			{
				const char *name = files.at(i).c_str();
				check_modifiers(name, name);
			}
			else if (relative == 2)
			{
				const char *name = files.at(i).c_str();
				char *modname = new char[sizeof(files.at(i).c_str()) + 8];
				strcpy(modname, "./");
				strcat(modname, name);
				strcat(modname, "\0");
				check_modifiers(modname, name);
				delete[] modname;
			}
			else
			{
				const char *name = files.at(i).c_str();
	//			char *modname = argv[index];
	//			modname = strcat(modname, name);
				const char *arg = argv[index];
				
				char *modname = new char[sizeof(files.at(i).c_str()) + sizeof(argv[index]) + 32];
				//char *modarg = new char[sizeof(files.at(i).c_str()) + sizeof(argv[index]) + 1];
				strcpy(modname, arg);
				strcat(modname, "/");
				strcat(modname, name);
				strcat(modname, "\0");
	//				cout << "modname: " << modname << endl << flush;
				check_modifiers(modname, name);
				delete[] modname;
				//delete[] modarg;
			}
		}
		//additional info like user, machine, additional stuff
	}
	

//TODO: implement the recursive function...ew, recursion

//TODO: create a case where a file is passed in as a parameter

//TODO: create a case where a directory is passed in without any flags

//TODO: create a default case where no parameters are given


	if ((index == 1))
	{
		const char *dirName = "./";
		if (argv[index] == 0)
		{
			dirName = "./";
		}
		else
		{
			dirName = argv[index];
		}

		check_dir(dirName, files);
		sort(files.begin(), files.end());
		print_files(files, 0, false);
	}



	return 0;
}


void print_files(vector<string> files, int start, bool hidden)
{
	if (hidden)
	{

		for (unsigned i = start; i < files.size(); i++)
		{
			cout << setw(files.at(i).size()) << files[i] << "  ";
		}
	}
	else
	{	
		for (unsigned i = 0; i < files.size(); i++)
		{
			
			if (files.at(i).at(0) != '.')
			{
				cout << setw(files.at(i).size()) << files.at(i) << "  " << flush;
			}
			/*
			if (((i % 10) == 0) && (i != 0))
			{
				cout << endl;
			}
			*/
		}
	}
	cout << endl;

	
}

void check_modifiers(const char* name, const char* realname)
{
	struct stat statbuf;
	int color = 0;
	errno = 0;
//	cout << argv[index][0] << endl;
//	if ((argv[index][0] == '/') && (argv[index] != NULL))
//	{
//		char* modname = argv[index];
//		name = strcat(modname, name);
//	}
//	cout << "name: " << name <<endl <<flush;
	stat(name, &statbuf);
	if(errno == 0)
	{

		//directory/symbolic link
		if(S_ISDIR(statbuf.st_mode))
		{
			cout << "d";
			color = 1;		//directory
		}
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
		{
			cout << "x";
			color = 2;		//executeable
		}
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
		{
			cout << "x";		//executeable
			color = 2;
		}
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
		cout << " " << flush;	
		cout << setw(3) << left  << statbuf.st_nlink << flush; //number of symbolic links

		errno = 0;
		struct passwd *p;
		p = getpwuid(statbuf.st_uid);
		if((errno == 0) && (p != NULL))
		{
			cout << setw(8) << left << p->pw_name << " " << flush;  //print out username based on UID
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
			cout << setw(8) << left << g->gr_name << flush;  //print out group name based on GID
		}

		cout << setw(sizeof(statbuf.st_size)) << left << statbuf.st_size; //print out size in bytes

		struct tm *localTime;
		time_t t = statbuf.st_mtime;
		if((localTime = localtime(&t)) != NULL)
		{
			char timestr[100];
			if((strftime(timestr, sizeof(timestr), "%b %e %H:%M", localTime)) != 0)
			{
				cout << setw(12) << left << timestr << flush;
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

		if (color == 1)
		{
			cout << "\x1b[32m"; //green
		}
		else if (color == 2)
		{
			cout << "\x1b[34m"; //blue
		}
		else
		{
			cout << "\x1b[0m"; //reset
		}
	//	if (argv[index][0] == '/')
	//		name++; //remove leading "/"
		cout << " " << realname << "\x1b[0m" << flush;  //location

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
		dirent *direntp;
		while (true)
		{
			errno = 0;
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
