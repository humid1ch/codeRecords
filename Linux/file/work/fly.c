#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define SEP "\R\N"

struct fly {
    int num;
    char addrStart[30];
    char addrEnd[30];
    char date[20];
    int price;
};

struct fly flys[] = {{1001, "武汉", "南京", "2024/1/16", 2000},
                     {1002, "秀湖", "南京", "2024/1/15", 4000},
                     {1003, "芜湖", "天津", "2024/1/17", 6000}};

void usage() {
    printf("usage: \n");
    printf("\t./flycp oldFile(NoDir) newFile(NoDir)\n");
}

int isWholeFly(const char* flys) {
    return strstr(flys, "\r") == NULL ? 0 : 1;
}

void saveFlyInfo(int fd, const struct fly* flys, int size) {
    for (int i = 0; i < size; i++) {
        char flyInfo[50] = {};
        sprintf(flyInfo, "%d-%s-%s-%s -%d\r\n", flys[i].num, flys[i].addrStart,
                flys[i].addrEnd, flys[i].date, flys[i].price);
        printf("%s", flyInfo);

        write(fd, flyInfo, strlen(flyInfo));
        fsync(fd);
    }
}

void readFlyInfo(int fdS, struct fly* flys, int size) {
    char buffer[1024] = {};
    int ret = 0;
    int i = 0;
    int offset = 0;
    while ((ret = read(fdS, buffer + offset, sizeof(buffer) - offset)) > 0) {
        offset = 0;
        char* flyInfo = strtok(buffer, "\n");
        printf("%s\n", flyInfo);
        while (flyInfo && strlen(flyInfo) > 0) {
            if (isWholeFly(flyInfo)) {
                flyInfo[strlen(flyInfo) - 1] = 0;
                sscanf(flyInfo, "%d-%s-%s-%s -%d", &flys[i].num, flys[i].addrStart,
                       flys[i].addrEnd, flys[i].date, &flys[i].price);
                i++;
                printf("%s\n", flyInfo);
                printf("%d\n", flys[i].price);
            }
            else {
                break;
            }
            flyInfo = strtok(NULL, "\n");
        }

        // 清空缓冲区
        memset(buffer, 0, sizeof(buffer));
        // 将flyInfo 存储到buffer中, 并计算偏移量
        if (flyInfo) {
            offset = strlen(flyInfo);
            memmove(buffer, flyInfo, strlen(flyInfo));
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        usage();
    }

    umask(000);

    int isExist = access(argv[1], F_OK);
    if (!isExist) {
        struct stat fileStat = {};
        stat(argv[1], &fileStat);
        if (S_ISDIR(fileStat.st_mode)) {
            usage();
            return -1;
        }
    }

    int fdS = open(argv[1], O_CREAT | O_RDWR, 0666);
    if (fdS == -1) {
        printf("%s open faild!\n", argv[1]);
        return -2;
    }

    int fdT = open(argv[2], O_CREAT | O_WRONLY, 0666);
    if (fdT == -1) {
        printf("%s open faild!\n", argv[2]);
        return -3;
    }

    saveFlyInfo(fdS, flys, sizeof(flys) / sizeof(flys[0]));

    struct fly newFlys[sizeof(flys) / sizeof(flys[0])] = {};
    lseek(fdS, 0, SEEK_SET);
    readFlyInfo(fdS, newFlys, sizeof(flys) / sizeof(flys[0]));

    saveFlyInfo(fdT, newFlys, sizeof(newFlys) / sizeof(newFlys[0]));

    close(fdS);
    close(fdT);

    return 0;
}
