#include <stdio.h>

#ifndef _WIN32
void strupr(char *);
#endif
void catcher(int);
#define FIXME(args) fprintf(stderr, "%s\n", args)

