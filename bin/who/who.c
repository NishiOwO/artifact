/*
 * who
 */

#include <stdio.h>
#if defined(__linux__) || defined(__OpenBSD__)
#include <utmp.h>
#else
#include <utmpx.h>
#endif
#include <pwd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#if defined(__linux__) || defined(__OpenBSD__)
struct utmp utmp;
#else
struct utmpx utmp;
#endif
struct passwd *pw;

int putline();

#if defined(__linux__) || defined(__FreeBSD__)
#define ut_name ut_user
#endif

int
main(argc, argv)
int argc;
char **argv;
{
	register char *tp, *s;
	register FILE *fi;

#if defined(__linux__) || defined(__OpenBSD__)
	s = "/var/run/utmp";
#else
	s = "/var/run/utmpx";
#endif
	if(argc == 2)
		s = argv[1];
	if (argc==3) {
		tp = ttyname(0);
		if (tp)
			tp = index(tp+1, '/') + 1;
		else {	/* no tty - use best guess from passwd file */
#ifdef __OpenBSD__
			time_t t = utmp.ut_time;
#else
			time_t t = utmp.ut_tv.tv_sec;
#endif
			pw = getpwuid(getuid());
			strcpy(utmp.ut_name, pw?pw->pw_name: "?");
			strcpy(utmp.ut_line, "tty??");
			time(&t);
			putline();
			exit(0);
		}
	}
	if ((fi = fopen(s, "r")) == NULL) {
		puts("who: cannot open utmp");
		exit(1);
	}
	while (fread((char *)&utmp, sizeof(utmp), 1, fi) == 1) {
		if(argc==3) {
			if (strcmp(*(char**)&utmp.ut_line, tp))
				continue;
#ifdef interdata
			printf("(Interdata) ");
#endif
			putline();
			exit(0);
		}
		if(utmp.ut_name[0] == '\0' && argc==1)
			continue;
		putline();
	}
}

int
putline()
{
	register char *cbuf;
#ifdef __OpenBSD__
	time_t t = utmp.ut_time;
#else
	time_t t = utmp.ut_tv.tv_sec;
#endif

	printf("%-8.8s %-8.8s", utmp.ut_name, utmp.ut_line);
	cbuf = ctime(&t);
	printf("%.12s\n", cbuf+4);
}
