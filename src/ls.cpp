#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
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
	
//TODO: implement whatever this is	
//	if (flags & FLAG_a && curfile[0] == '.')
	{
		
	}	

	return 0;
}
