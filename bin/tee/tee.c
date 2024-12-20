/*
 * tee-- pipe fitting
 */

#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int openf[20] = { 1 };
int n = 1;
int t = 0;
int aflag;

char in[512];

char out[512];

int stash();

int
main(argc,argv)
int argc;
char **argv;
{
	int register r,w,p;
	struct stat buf;
	while(argc>1&&argv[1][0]=='-') {
		switch(argv[1][1]) {
		case 'a':
			aflag++;
			break;
		case 'i':
		case 0:
			signal(SIGINT, SIG_IGN);
		}
		argv++;
		argc--;
	}
	fstat(1,&buf);
	t = (buf.st_mode&S_IFMT)==S_IFCHR;
	if(lseek(1,0L,1)==-1&&errno==ESPIPE)
		t++;
	while(argc-->1) {
		if(aflag) {
			openf[n] = open(argv[1],1);
			if(openf[n] < 0)
				openf[n] = creat(argv[1],0666);
			lseek(openf[n++],0L,2);
		} else
			openf[n++] = creat(argv[1],0666);
		if(stat(argv[1],&buf)>=0) {
			if((buf.st_mode&S_IFMT)==S_IFCHR)
				t++;
		} else {
			puts("tee: cannot open ");
			puts(argv[1]);
			puts("\n");
			n--;
		}
		argv++;
	}
	r = w = 0;
	for(;;) {
		for(p=0;p<512;) {
			if(r>=w) {
				if(t>0&&p>0) break;
				w = read(0,in,512);
				r = 0;
				if(w<=0) {
					stash(p);
					return 0;
				}
			}
			out[p++] = in[r++];
		}
		stash(p);
	}
}

int
stash(p)
int p;
{
	int k;
	int i;
	int d;
	d = t ? 16 : p;
	for(i=0; i<p; i+=d)
		for(k=0;k<n;k++)
			write(openf[k], out+i, d<p-i?d:p-i);
}
