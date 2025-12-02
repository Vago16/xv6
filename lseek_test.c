#include "types.h"
#include "user.h"
#include "fcntl.h"

//test file for lseek function

int main() {
  //test movement of spaces
  int fd = open("testfile.txt", O_CREATE | O_RDWR);

  write(fd, "Hello", 5);

  lseek(fd, 10); //move 10 spaces past offset 5, so now at 15

  write(fd, "World", 5);

  close(fd);

  fd = open("testfile.txt", O_RDONLY);

  char buf[25];
  int n = read(fd, buf, sizeof(buf));

  //skipping holes to print "Hello" and "World" concatenated
  for(int i = 0; i < n; i++) {
        if(buf[i] != 0)
            printf(1, "%c", buf[i]);
    }
    printf(1, "\n");

  close(fd);

  //test filling holes in the file with zero
  int fd_2 = open("hole.txt", O_CREATE | O_RDWR);
  write(fd_2, "A", 1);      
  lseek(fd_2, 5);            
  write(fd_2, "B", 1);          //now size = 7, with 66 showing 5 spaces offset
  close(fd_2);

  fd_2 = open("hole.txt", 0);
  char buf_2[7];
  read(fd_2, buf_2, 11);

  //print each byte as integer
  for(int i = 0; i < 7; i++)
      printf(1, "%d ", buf_2[i]);
  printf(1, "\n");

  exit();
}
