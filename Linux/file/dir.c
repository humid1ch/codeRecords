#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc, char* argv[]) {
    DIR* dir = opendir(argv[1]);
    if (!dir) {
        printf("open dir: %s faild.\n", argv[1]);
        return -1;
    }

    printf("open dir: %s success.\n", argv[1]);

    closedir(dir);

    return 0;
}
