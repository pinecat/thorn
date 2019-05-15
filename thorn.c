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

    // parse flags
    char *flags = parse_flags(argc, argv);

    // parse filepath
    char *path = parse_path(argc, argv);
    if (path == -1) {
        PRT_USG;
        return 1;
    }

    // dump the hex values and get the total bytes read in
    int tbytes = hex_dump(path, flags);
    if (tbytes == -1) { return 1; }

    // print total bytes
    if (!flags[0]) { // if not in clean mode
        printf("\nTotal Bytes: %d\n", tbytes);
    }

    // free flags
    free(flags);

    // quit program cleanly
    return 0;
}

/*
    parse_path
*/
char *parse_path(int argc, char **argv)
{
    int i; // index to loop through argv
    for (i = 1; i < argc; i++) { // loop until end of arguments, not getting program name
        if (argv[i][0] != '-') { // if the current argument does not have a dash
            if (argv[i - 1][1] != 'l') { // if the argument before it was not -l
                return argv[i]; // then return the file
            }
        }
    }
    return -1; // otherwise return -1
}

/*
    parse_flags
*/
char *parse_flags(int argc, char **argv)
{
    // get char array ready to hold flags
    char *flags = malloc(sizeof(char) * 4); // initialize array
    flags[0] = 0; // set the CLEAN flag to 0
    flags[1] = 0; // set the NOASCII flag to 0
    flags[2] = 0; // set the HEXPREFIX flag to 0
    flags[3] = NUM_PER_LINE; // set NPERLINE flag to NUM_PER_LINE (default 16)

    int i; // index to loop through argv
    for (i = 1; i < argc; i++) { // loop through argv starting after program name
        if (argv[i][0] == '-') { // if the argument begins with a -
            char f = argv[i][1]; // get the arg letter

            if (f == 'h') { // h - help menu
                help();
                exit(0);
            } else if (f == 'c') { // c - clean mode
                flags[0] = 1;
            } else if (f == 'n') { // n - no ascii mode
                flags[1] = 1;
            } else if (f == 'x') { // x - hex prefix ('0x') mode
                flags[2] = 1;
            } else if (f == 'l') { // l - specify number of values per line
                if (i < argc - 1) { // check if there is still another value
                    flags[3] = atoi(argv[i+1]); // if there is, convert it to number and store
                } else { // otherwise print usage and exit
                    PRT_USG;
                    exit(1);
                }
            } else { // otherwise, if a different character is detected, print usage and exit
                PRT_USG;
                exit(1);
            }
        }
    }

    // return flags
    return flags;
}

/*
    help
*/
void help()
{
    printf("NAME\n");
    printf("\tthorn - a simple program to dump the hex codes from binary files\n\n");

    printf("USAGE\n");
    printf("\tthorn [-c] [-n] [-x] [-l nperline] <filename>\n\n");

    printf("DESCRIPTION\n");
    printf("\tThis program simply dumps hex values from binary files.\n\tThe name comes from the lead guitarist of the Hex Girls in Scooby Doo (Thorn of course :]).\n\n");

    printf("OPTIONS\n");
    printf("\tThe program may be called with a number of flags/options which are shown below.\n\n");

    printf("\t-c\n");
    printf("\t\tClean output.  No ascii, no byte counter, just the hex values.\n\n");

    printf("\t-n\n");
    printf("\t\tNo ascii mode.  Won't print out ascii values, but will still print byte counter.\n\n");

    printf("\t-x\n");
    printf("\t\tHex format.  Will print out '0x' before each hex vale.\n\n");

    printf("\t-l\n");
    printf("\t\tSpecify the number of values per line.  If not specified, a default value of 16 will be used.\n\n");

    printf("EXIT STATUS\n");
    printf("\tthorn exits with 0 on a clean quit, and 1 if an error occurred.\n\n");

    printf("AUTHOR\n");
    printf("\trory (aka pinecat)\n\n");
}

/*
    hex_dump
*/
int hex_dump(char *path, char *flags)
{
    // get # of values to print out per line
    int n = flags[3];

    /* open file */
    int fd = open(path, O_RDONLY); // open file as read only
    if (fd == -1) { // if file is not open, print message and return/exit program
        printf("thorn: %s: No such file or directory\n", path); // error message
        return -1; // return value of -1
    }

    /* set file to binary mode for reading (only on Windows) */
    #ifdef _WIN32
    _setmode(fd, 0x8000); // 0x8000 = _O_BINARY
    #endif

    /* reading file & printing values */
    unsigned char *buffer = malloc(sizeof(unsigned char) * n); // buffer for reading file
    int bytes = 0; // variable to store bytes read
    int tbytes = 0; // variable to store total bytes read

    while ((bytes = read(fd, buffer, sizeof(unsigned char) * n)) > 0) {
        if (!flags[0]) { // if not in clean mode
            printf("0x%08x    ", tbytes); // print out bytes counter
        }

        int i; // index for buffer
        for (i = 0; i < bytes; i++) { // loop through buffer for hex values
            // print hex
            if (!flags[2]) {
                printf("%02x ", buffer[i]); // print out hex value
            } else {
                printf("0x%02x ", buffer[i]); // print out hex value with '0x' prefix
            }
        }

        if (!flags[0] && !flags[1]) { // if not in clean mode
            printf("    "); // formatting
            int i; // index for buffer
            for (i = 0; i < bytes; i++) { // loop through buffer for ascii values
                if (isprint(buffer[i])) { // if the character is printable...
                    printf("%c", buffer[i]); // ...print it
                } else { // else...
                    printf("."); // ...just print a period
                }
            }
        }

        tbytes += bytes; // sum total bytes with bytes

        printf("\n");
    }

    // close the file
    close(fd);

    // return total bytes read
    return tbytes;
}
