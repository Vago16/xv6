#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    if (argc < 2) {     //gotten from rm.c
        printf(2, "Error: Pass argument for ticks\n");
        exit();
    }
    int ticks = atoi(argv[1]);     //convert command line arg to an integer using atoi() as shown in ulib.c
    sleep(ticks);        //syscall hello
    exit();
}