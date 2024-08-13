#include <string.h>
#include <stdio.h>

int main() {
    char a = 0;
    char* str = &a;
    strcpy(str, "hello");
    printf("%s\n", str);

    return 0;
}
