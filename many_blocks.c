#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define BSIZE 512
#define BLOCKS 32906
#define SUCCESS 16523
//test for blocks in file size

int main() {
    //create file to hole 16523 blocks
    int fd = open("bigfile", O_CREATE | O_WRONLY);
    if (fd < 0) {
        printf(1, "Failed to create file\n");
        exit();
    }

    char buf[BSIZE];
    for (int i = 0; i < BSIZE; i++) {
        buf[i] = 'A';
    }

    //iterate through blocks, print where block failed if failure
    for (int i = 0; i < BLOCKS; i++) {
        int n = write(fd, buf, BSIZE);

        if (n != BSIZE) {
            printf(1, "Write FAILED at block %d (returned %d)\n", i, n);
            break;
        }

        if (i == SUCCESS - 1)
            printf(1, "Reached expected max block limit: %d (SUCCESS)\n", i + 1);
    }

    printf(1, "Finished writing\n");
    close(fd);
    exit();
}
