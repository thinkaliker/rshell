#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
using namespace std;

#define FLAG_a 1
#define FLAG_l 2
#define FLAG_R 4

bool is_directory(char* path);

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

//TODO: implement whatever this is which has something to do with hidden files
	if (flags & FLAG_a && curfile[0] == '.')
	{
		
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

//TODO: create a default case where no parameters are given
	
	{
		char *dirName = ".";
		DIR *dirp;
		if ((*dirp = opendir(dirName)) == -1)
		{
			perror("opendir");
			exit(1);
		}
		dirent *direntp;
		while ((direntp = readdir(dirp)))
			cout << direntp->d_name << endl;
		closedir(dirp);
	}
	

	return 0;
}

bool is_directory(char* path)
{
	//function to check if a path is a directory or not
}

void check_mod(stat statbuf)
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
