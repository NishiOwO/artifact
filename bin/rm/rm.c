int	errcode;

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef __FreeBSD__
#include	<dirent.h>
#define		direct dirent
#else
#include	<sys/dir.h>
#endif
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void rm();
int yes();
int dotname();

int
main(argc, argv)
int argc;
char *argv[];
{
	register char *arg;
	int fflg, iflg, rflg;

	fflg = 0;
	if (isatty(0) == 0)
		fflg++;
	iflg = 0;
	rflg = 0;
	if(argc>1 && argv[1][0]=='-') {
		arg = *++argv;
		argc--;
		while(*++arg != '\0')
			switch(*arg) {
			case 'f':
				fflg++;
				break;
			case 'i':
				iflg++;
				break;
			case 'r':
				rflg++;
				break;
			default:
				printf("rm: unknown option %s\n", *argv);
				exit(1);
			}
	}
	while(--argc > 0) {
		if(!strcmp(*++argv, "..")) {
			fprintf(stderr, "rm: cannot remove `..'\n");
			continue;
		}
		rm(*argv, fflg, rflg, iflg, 0);
	}

	exit(errcode);
}

void
rm(arg, fflg, rflg, iflg, level)
char arg[];
int fflg, rflg, iflg, level;
{
	struct stat buf;
	struct direct direct;
	char name[100];
	int d;

	if(stat(arg, &buf)) {
		if (fflg==0) {
			printf("rm: %s nonexistent\n", arg);
			++errcode;
		}
		return;
	}
	if ((buf.st_mode&S_IFMT) == S_IFDIR) {
		if(rflg) {
			if (access(arg, 02) < 0) {
				if (fflg==0)
					printf("%s not changed\n", arg);
				errcode++;
				return;
			}
			if(iflg && level!=0) {
				printf("directory %s: ", arg);
				if(!yes())
					return;
			}
			if((d=open(arg, 0)) < 0) {
				printf("rm: %s: cannot read\n", arg);
				exit(1);
			}
			while(read(d, (char *)&direct, sizeof(direct)) == sizeof(direct)) {
				if(direct.d_ino != 0 && !dotname(direct.d_name)) {
					sprintf(name, "%s/%.14s", arg, direct.d_name);
					rm(name, fflg, rflg, iflg, level+1);
				}
			}
			close(d);
			errcode += rmdir(arg);
			return;
		}
		printf("rm: %s directory\n", arg);
		++errcode;
		return;
	}

	if(iflg) {
		printf("%s: ", arg);
		if(!yes())
			return;
	}
	else if(!fflg) {
		if (access(arg, 02)<0) {
			printf("rm: %s %o mode ", arg, buf.st_mode&0777);
			if(!yes())
				return;
		}
	}
	if(unlink(arg) && (fflg==0 || iflg)) {
		printf("rm: %s not removed\n", arg);
		++errcode;
	}
}

int
dotname(s)
char *s;
{
	if(s[0] == '.')
		if(s[1] == '.')
			if(s[2] == '\0')
				return(1);
			else
				return(0);
		else if(s[1] == '\0')
			return(1);
	return(0);
}

int
yes()
{
	int i, b;

	i = b = getchar();
	while(b != '\n' && b != EOF)
		b = getchar();
	return(i == 'y');
}
