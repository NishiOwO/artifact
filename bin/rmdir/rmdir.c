/*
 * Remove directory
 */

#include <sys/types.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	Errors = 0;
char	*rindex();

int
main(argc,argv)
int argc;
char **argv;
{

	if(argc < 2) {
		fprintf(stderr, "rmdir: arg count\n");
		exit(1);
	}
	while(--argc)
		rmdir(*++argv);
	exit(Errors!=0);
}
