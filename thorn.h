#ifndef THORN_H
#define THORN_H

/* imports */
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/* consts */
#define BUF_SIZE 512
#define NUM_PER_LINE 8

/* macros */
#define PRT_USG printf("Usage: thorn [-a] [-l nperline] <filename>\n")

/* prototypes */
int hex_dump(char *path);

#endif
