#ifndef THORN_H
#define THORN_H

/* imports */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* consts */
#define NUM_PER_LINE 16

/* macros */
#define PRT_USG printf("Usage: thorn [-c] [-n] [-x] [-l nperline] <filename>\n")

/* prototypes */
char *parse_path(int argc, char **argv);
char *parse_flags(int argc, char **argv);
void help();
int hex_dump(char *path, char *flags);

#endif
