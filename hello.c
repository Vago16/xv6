//Part 1: Writing a Hello World Program in Xv6
//hello.c file that outputs "Hello Xv6! to terminal when called

#include "types.h"
#include "user.h"


//copied from echo.c with some changes
int
main(int argc, char *argv[])
{
  printf(1, "Hello Xv6!\n");        //1st arg is the file descriptor,1 is the output to stdout
  hello();    //syscall from kernel
  exit();       //no args in xv6
}