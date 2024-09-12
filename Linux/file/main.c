#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
    umask(000);
    int fdS = open("./source", O_CREAT | O_RDONLY, 0666);
    if (fdS == -1) {
        return -1;
    }

    int fdT = open("./target", O_CREAT | O_WRONLY | O_APPEND, 0666);
    if (fdT == -1) {
        return -1;
    }

    char buf[1024] = {};
    int ret = 0;
    while ((ret = read(fdS, buf, sizeof(buf))) > 0) {
        printf("%d %s", ret, buf);
        write(fdT, buf, sizeof(ret));
        fsync(fdT);
        memset(buf, 0, sizeof(buf));
    }

    close(fdS);
    close(fdT);

    return 0;
}
