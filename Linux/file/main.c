#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    umask(000);
    int fd = open("./test.txt", O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        return -1;
    }

    char readR[20] = {};

//     write(fd, "hello world!!!!!", 15);
//     fsync(fd);
//     printf("%d\n", fd);

    int ret = read(fd, readR, 11);
    fsync(fd);
    printf("%d %s", ret, readR);

    close(fd);

    return 0;
}
