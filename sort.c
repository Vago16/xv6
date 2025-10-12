#include "types.h"
#include "stat.h"
#include "user.h"
//sort command file

char lines_to_read[512][128];   //global array of max number of lines and max number of characters per line to prevent memory overflow


int
main(int argc, char *argv[])
{
    int fd = 0; //file descriptor, 0 for standard input by default
    int num = 0; //number of lines read so far
    char c; //stores character read from file
    char buf[128];  //buffer to store a line
    int index = 0;  //the index stored in the buffer

    //flag variables, flips to 1 if detected in input
    int reverse = 0;
    int numerical = 0;
    int unique = 0;

    //open file and check for flags
    if(argc > 1) {
        for (int i = 1; i < argc; i++) { 
            if(strcmp(argv[i], "-r") == 0) {
                // -r flag recogntion
                reverse = 1;
            } else if (strcmp(argv[i], "-n") == 0) {
                // -n flag recogntion
                numerical = 1;
            } else if (strcmp(argv[i], "-u") == 0) {
                // -n flag recogntion
                unique = 1;
            } else {
            //open file normally if no flags
            fd = open(argv[i], 0);  //open provided file as argument for reading
            } 
        }
    }
    if (fd < 0) {
        //error if cannot open file
        printf(1, "File can not be opened");
        exit();
    }
    //read 1 byte at a time from file descriptor till end of file(null character) or error(negative value)
    while (read(fd, &c, 1) == 1) {
        if (c == '\n') {    //if end of line
            buf[index] = 0; //put null terminator
            strcpy(lines_to_read[num++], buf); //copy buffer to next line
            index = 0; //go back to beginning of next line
            if (num >= 512) {
                break;  //stop if max amount of lines allowed is reached
            }
        } else {
            if (index < 128 - 1) {  //if not end of line
                buf[index++] = c;   //store character read in c from next index spot in line
            }
        }
    }

    //runs through flag recognition to see which sort to use
    if (reverse) {
        //bubble sort for "-r" flag
        for(int i = 0; i < num - 1; i++) {
            for(int j = i + 1; j < num; j++) {
                //if i is smaller than j(in this case earlier in alphabet), swap them in order
                if (strcmp(lines_to_read[i], lines_to_read[j]) < 0) {
                    char temp[128];  //temporary variable to store first variable to be swapped into
                    strcpy(temp, lines_to_read[i]);
                    strcpy(lines_to_read[i], lines_to_read[j]);
                    strcpy(lines_to_read[j], temp);
                }   
            }
        }
    } else if (numerical) {
        //bubble sort for "-n" flag
        for(int i = 0; i < num - 1; i++) {
            for(int j = i + 1; j < num; j++) {
                int a = atoi(lines_to_read[i]);
                int b = atoi(lines_to_read[j]);
                //if i is bigger than j, swap them in order
                if (a > b) {
                    char temp[128];  //temporary variable to store first variable to be swapped into
                    strcpy(temp, lines_to_read[i]);
                    strcpy(lines_to_read[i], lines_to_read[j]);
                    strcpy(lines_to_read[j], temp);
                }   
            }
        }
    } else if (unique) {
        //bubble sort for "-u" flag, starts with normal bubble sort, then remove duplicate lines
        for(int i = 0; i < num - 1; i++) {
            for(int j = i + 1; j < num; j++) {
                //if i is larger than j(in this case means later in alphabet), swap them in order
                if (strcmp(lines_to_read[i], lines_to_read[j]) > 0) {
                    char temp[128];  //temporary variable to store first variable to be swapped into
                    strcpy(temp, lines_to_read[i]);
                    strcpy(lines_to_read[i], lines_to_read[j]);
                    strcpy(lines_to_read[j], temp);
                }   
            }
        }
        //variable of index to make sure lines are unique
        int unique_line = 0;
        //if looking at first line, or the line being looked at is not equal to the line before it, add to unique_line
        for (int i = 0; i < num; i++) {
            if (i == 0 || strcmp(lines_to_read[i], lines_to_read[i - 1]) != 0) {
                strcpy(lines_to_read[unique_line], lines_to_read[i]);
            }
        }
        num = unique_line;   //now shows number of unique lines
    } else {
        //no flags
        //simple bubble sort to handle actual sorting of lines in aphabetical order in 2-D array for text files
        for(int i = 0; i < num - 1; i++) {
            for(int j = i + 1; j < num; j++) {
                //if i is larger than j(in this case means later in alphabet), swap them in order
                if (strcmp(lines_to_read[i], lines_to_read[j]) > 0) {
                    char temp[128];  //temporary variable to store first variable to be swapped into
                    strcpy(temp, lines_to_read[i]);
                    strcpy(lines_to_read[i], lines_to_read[j]);
                    strcpy(lines_to_read[j], temp);
                }   
            }
        }
    }
    //loop through lines and prints them out to standard output(terminal)
    for (int i = 0; i < num; i++) {
        printf(1, "%s\n", lines_to_read[i]) ;    //1 is for standard output
    }
    //close file
    if (fd != 0) {
        close(fd);
    }

    exit();
}
