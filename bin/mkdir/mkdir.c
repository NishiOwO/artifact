/*
** make directory
*/

#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<sys/stat.h>

int	Errors = 0;

int
main(argc, argv)
int argc;
char *argv[];
{

	signal(SIGHUP, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	signal(SIGTERM, SIG_IGN);

	if(argc < 2) {
		fprintf(stderr, "mkdir: arg count\n");
		exit(1);
	}
	while(--argc)
		mkdir(*++argv, 0755);
	exit(Errors!=0);
}
