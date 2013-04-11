#include <unistd.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/mman.h> 
#include <fcntl.h> 
#include <stdlib.h> 

#define SIZE ( 2188990) 
//# define SIZE ( 218000) 

int main(int argc, char * argv[]) 
{ 
	char * p; 
	int i, fd; 
	FILE * fp; 
	int lines = 0;
	int delay = 100;

	fd = open ("records.dat" , O_RDWR) ; 
	p = (char * ) mmap( 0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0) ; 

	if (argc > 1)
		delay = atoi(argv[1]);

	int j = 0;
	for (i = 0; i < SIZE; i++)
	{
		char line[128];

		// normal char, just print it 
		if (p[i] != '\\')
		{
			line[j++] = p[i];
			continue;
		}

		// deal with \\, \n, \'
		if (p[i+1] == 'n')
		{
			line[j++] = '\n';
			line[j] = '\0';

			// skip the line only show a number
			if (strlen(line) > 0 && lines % 14 != 0)
				printf("%s", line);

			j = 0;
			lines++;

		}
		else
			line[j++] = p[i+1];

		i++;
		if (lines % 14 == 0)
		{
			printf("frame %d\n", lines / 14);
			usleep(delay*1000);
			//system("reset");
			system("clear");
			continue;
		}

	}

	munmap((void *)p, SIZE ) ; 
	printf("Ok, the Star War show is over.\n"); 
	close(fd) ; 

	return 0; 

}
