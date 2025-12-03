#define O_RDONLY  0x000
#define O_WRONLY  0x001
#define O_RDWR    0x002
#define O_CREATE  0x200
#define O_NOFOLLOW 0x400    //from manpages, If the trailing component (i.e., basename) of path is a symbolic link, then the open fails, with the error ELOOP