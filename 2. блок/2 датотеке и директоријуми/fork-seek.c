#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>    // getopt
#include <unistd.h>    // lseek, read
#include<sys/wait.h>

int main(int argc, char *argv[]) {
    int fd = open("tmp.txt", O_CREAT|O_RDONLY);
    assert(fd >= 0);
    int rc = fork();
    if (rc == 0) {
        rc = lseek(fd, 10, SEEK_SET);
        printf("DETE: offset %d\n", rc);
    }
    else if (rc > 0) {
        (void) wait(NULL);
        printf("RODITELJ: offset %d\n", (int)lseek(fd, 0, SEEK_CUR));
    }
    return 0;
}