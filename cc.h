#ifndef HEADER_cc
#define HEADER_cc

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define MAXFIL 100

#define ASSERT(x, s) if (x) fatal(s);

#define ARRAY_ASSIGN(type, arr, ...)                    \
        {                                               \
                type temp[] = { __VA_ARGS__ };          \
                memcpy(arr, temp, sizeof(temp));        \
        }

#define OPTIONS                 \
        OPTION(help,    0)      \
        OPTION(o,       1)      \
        OPTION(S,       2)      \
        OPTION(E,       3)
#define OPTCOUNT 4

extern int      getopt(const char *);
extern char     *_(const char *, ...);
extern void     error(const char *);
#define fatal(s) { error(s); exit(1); }
extern int      getsuf(char *);
extern char     *setsuf(char *, char);
extern int      callsys(char *, char **);

extern char     *outfile;
extern char     *clist[MAXFIL];
extern char     *filename;
extern char     *av[50];
extern int      exflag;
extern int      cflag;
extern int      eflag;
extern int      oflag;
extern int      sflag;
extern int      nerror;

#endif /* HEADER_cc */