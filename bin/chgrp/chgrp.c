/*
 * chgrp gid file ...
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <grp.h>

struct	group	*gr;
struct	stat	stbuf;
int	gid;
int	status;
int	isnumber();

int
main(argc, argv)
int argc;
char *argv[];
{
	register int c;

	if(argc < 3) {
		printf("usage: chgrp gid file ...\n");
		exit(4);
	}
	if(isnumber(argv[1])) {
		gid = atoi(argv[1]);
	} else {
		if((gr=getgrnam(argv[1])) == NULL) {
			printf("unknown group: %s\n",argv[1]);
			exit(4);
		}
		gid = gr->gr_gid;
	}
	for(c=2; c<argc; c++) {
		stat(argv[c], &stbuf);
		if(chown(argv[c], stbuf.st_uid, gid) < 0) {
			perror(argv[c]);
			status = 1;
		}
	}
	exit(status);
}

int
isnumber(s)
char *s;
{
	register int c;

	while(c = *s++)
		if(!isdigit(c))
			return(0);
	return(1);
}
