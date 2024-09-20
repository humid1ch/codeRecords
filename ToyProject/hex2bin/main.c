#include <stdio.h>

void usage() {
    printf("The program's usage:\n");
    printf("\t./hex2bin HexFilePath NewBinFilePath\n");
}

int main(int argc, const char* argv[]) {
    if (argc != 3) {
        usage();

        return -1;
    }

    FILE* hexFile = fopen(argv[1], "rb"); // 二进制只读打开文件
    if (hexFile == NULL) {
        printf("hexFile open failed!  Please confirm that the file exists!\n\n");
        usage();

        return -2;
    }

    FILE* newBinFile = fopen(argv[2], "w+b"); // 二进制读写形式打开文件
    if (newBinFile == NULL) {
        printf("newBinFile failed to open for unknow reasons. \n");
        usage();

        return -3;
    }

    char hexBuffer[256] = {};
    while (fread(hexBuffer, sizeof(char), sizeof(hexBuffer), hexFile) != 0) {
        fwrite(hexBuffer, sizeof(char), sizeof(hexBuffer), newBinFile);
    }

    printf("HexFile to BinaryFile finish.\n");

    fclose(hexFile);
    fclose(newBinFile);

    return 0;
}
