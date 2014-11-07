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
		//need more code here about stuff


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
		
		//additional info like user, machine, additional stuff
	}
	

//TODO: implement the recursive function...ew, recursion


//TODO: create a default case where no parameters are given

	return 0;
}
