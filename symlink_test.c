#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "param.h"
//test file for symlink functionality

int main() {

    int fd;
    char buf[MAXPATH];

    fd = open("original.txt", O_CREATE | O_RDWR);
    if(fd < 0){
        printf(1, "Failed to create original.txt\n");
        exit();
    }
    write(fd, "HelloXV6", 8);
    close(fd);

    //create symlink to normal file
    if(symlink("original.txt", "link.txt") < 0){
        printf(1, "Failed to create symlink link.txt\n");
        exit();
    }

    //open symlink normally to read target file(without flag O_NOFOLLOW)
    fd = open("link.txt", O_RDONLY);
    if(fd < 0){
        printf(1, "Failed to open symlink link.txt\n");
        exit();
    }
    int n = read(fd, buf, sizeof(buf) - 1);
    buf[n] = 0;  // null-terminate
    printf(1, "Reading via symlink: %s\n", buf);
    close(fd);

    //open symlink with O_NOFOLLOW to read the symlink path itself
    fd = open("link.txt", O_RDONLY | O_NOFOLLOW);
    if(fd < 0){
        printf(1, "Failed to open symlink with O_NOFOLLOW\n");
        exit();
    }
    n = read(fd, buf, sizeof(buf) - 1);
    buf[n] = 0;
    printf(1, "Reading symlink target path: %s\n", buf);
    close(fd);

    exit();
}