#pragma pack(1)

#include <stdio.h>
#include <stddef.h>

struct student {
    char _group;
    int _age;
    char _name[9];
};

int main() {
    printf("student结构体大小为: %lu\n", sizeof(struct student));

    printf("\n_group 在 student内的偏移量为: %lu\n", offsetof(struct student, _group));

    printf("\n_age 在 student内的偏移量为: %lu\n", offsetof(struct student, _age));

    printf("\n_name 在 student内的偏移量为: %lu\n", offsetof(struct student, _name));

    return 0;
}
