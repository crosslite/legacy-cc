#include "cc.h"

enum {
#       define OPTION(s, i) __##s = i,
        OPTIONS
#       undef OPTION
};

int
main(int argc, char **argv)
{
        int i, c, nc;
        char *t, *assource;

        i = nc = 0;
        while (++i < argc) {
                if (*argv[i] == '-') switch (getopt(argv[i]+1)) {
                default:
                        goto passa;
                case __help:
                        puts("Usage: cc [options] file...");
                        exit(0);
                case __o:
                        if (argv[++i]==NULL || *argv[i]=='-')
                                fatal("missing filename after ‘-o’");
                        outfile = argv[i];
                        if ((c=getsuf(outfile))=='c' || c=='o')
                                fatal(_("would overwrite %s", outfile));
                        break;
                }
                else {
passa:
                        t = argv[i];
                        if ((c=getsuf(t))=='c' || c=='s' || exflag) {
                                clist[nc++] = t;
                                ASSERT(nc>=MAXFIL, "too many source files");
                                t = setsuf(t, 'o');
                        }
                }
        }
        ASSERT(nc<1, "no input files");
        if (nc==0)
                goto nocom;
        for (i=0; i<nc; i++) {
                if (getsuf(clist[i])=='s') {
                        assource = clist[i];
                        goto assemble;
                }
                av = build_argv(3, "c0", clist[i], 0);
                if (callsys("c0", av)) {
                        cflag++;
                        eflag++;
                        continue;
                }
                if (callsys("c1", av)) {
                        cflag++;
                        eflag++;
                        continue;
                }
                if (oflag) {
                        av = build_argv(3, "c0", clist[i], 0);
                        if (callsys("c2", av)) {

                        }
                }
                if (sflag)
                        continue;
assemble:
                av = build_argv(5, "as", assource, "-o", setsuf(clist[i], 'o'), 0);
                if (callsys("/usr/bin/as", av)) {
                        cflag++;
                        eflag++;
                        continue;
                }
        }
nocom:
        return(nerror!=0);
}

int
getopt(const char *s)
{
        int i;

#       define OPTION(s, i) #s,
        static char *arr[] = { OPTIONS };
#       undef OPTION
        for (i=0; i<OPTCOUNT; ++i)
                if (strcmp(s, arr[i])==0)
                        return i;
        return(-1);
}

char *
_(const char *format, ...)
{
        char *s;
        int len;
        va_list ap1, ap2;

        va_start(ap1, format);
        va_copy(ap2, ap1);
        len = vsnprintf(NULL, 0, format, ap1);
        s = malloc(++len);
        vsnprintf(s, len, format, ap2);

        va_end(ap1);
        va_end(ap2);
        return(s);
}

void
error(const char *s)
{
        if (filename==NULL)
                printf("cc: %s\n", s);
        else
                printf("cc: %s: %s\n", filename, s);
        nerror++;
}

int
getsuf(char *s)
{
        s = strrchr(s, '.');
        if (s && s[1]!='\0')
                return(s[1]);
        return(0);
}

char *
setsuf(char *s, char c)
{
        char *s1, *s2;

        s1 = s2 = malloc(strlen(s));
        strcpy(s1, s);
        s1 = strrchr(s1, '.');
        s1[1] = c;
        return(s2);
}

int
callsys(char *f, char **v)
{
        int t, status;

        if ((t=fork())==0) {
                execv(f, v);
                fatal(_("can't find %s", f));
        } else if (t==-1) {
                puts("try again");
                return(100);
        }
        while (t!=wait(&status))
                ;
        if (status>0) {
                if (status!=SIGINT) {
                        fatal(_("fatal error in %s", f));
                        eflag = 8;
                }
                exit(eflag);
        }
        return(status);
}

char **
build_argv(int count, ...)
{
    va_list args;
    int i;
    char **argv = malloc((count+1) * sizeof(char*));
    char *temp;

    va_start(args, count);
    for (i=0; i<count; i++) {
        temp = va_arg(args, char*);
        argv[i] = malloc(sizeof(temp));
        argv[i] = temp;
    }
    argv[i] = NULL;
    va_end(args);
    return(argv);
}

char    *outfile;
char    *clist[MAXFIL];
char    *filename       = NULL;
char    **av;
int     exflag          = 0;
int     cflag           = 0;
int     eflag           = 0;
int     oflag           = 0;
int     sflag           = 0;
int     nerror          = 0;