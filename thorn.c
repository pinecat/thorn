/*******************************************************
*   thorn
*   Desc: Simple hex dumping program for binary files
*   Files: thorn.c, thorn.h
*   Author: rory (pinecat)
*******************************************************/

/* imports */
#include "thorn.h"

/* main */
int main(int argc, char **argv)
{
    // check input before grabbing filename
    if (argc < 2) { // if there are less than 2 arguments (i.e. just the program name)
        PRT_USG; // print usage
        return 1; // quit program with exit code 1
    }

    // dump the hex values and get the total bytes read in
    int tbytes = hex_dump(argv[1]);

    // print total bytes
    printf("\nTotal Bytes: %d\n", tbytes);

    // quit program cleanly
    return 0;
}

/*
    hex_dump
*/
int hex_dump(char *path)
{
    /* open file */
    int fd = open(path, O_RDONLY); // open file as read only
    if (fd == -1) { // if file is not open, print message and return/exit program
        printf("thorn: %s: No such file or directory\n", path); // error message
        return -1; // return value of -1
    }

    /* set file to binary mode for reading */
    _setmode(fd, _O_BINARY);

    /* reading file & printing values */
    unsigned char buffer[BUF_SIZE];
    int bytes = 0;
    int tbytes = 0;

    while ((bytes = read(fd, buffer, sizeof(unsigned char) * BUF_SIZE)) > 0) {
        int i; // index for buffer
        int rcount = 0; // index for row count
        for (i = 0; i < bytes; i++) { // loop through buffer
            printf("0x%02x ", buffer[i]); // print out hex value
            rcount++; // increase row count
            if (rcount > NUM_PER_LINE) { // if row count is greater than the default...
                printf("\n"); // ...print a new line
                rcount = 0; // and reset the row count
            }
        }
        printf("\n"); // must do a final newline before next buffer
        tbytes += bytes; // sum total bytes with bytes
    }

    // close the file
    close(fd);

    // return total bytes read
    return tbytes;
}
