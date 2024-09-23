#include "seqlist.h"
#include <stdio.h>

int main() {
	Array arr;
	arrayInit(&arr, 10);

	for (int i = 0; i < 50; i++) {
		if (arr._size == arr._capacity) {
			printf("arr capacity: %lu, size: %lu\n", arr._capacity, arr._size);
			for (int i = 0; i < arr._size; i++) {
				printf("%d ", arr._data[i]);
			}
			printf("\n");
			printf("\n");
		}

		arrayPush(&arr, i);
	}

    arrayPop(&arr);
    arrayShow(&arr);

    arrayErase(&arr, 44);
    arrayShow(&arr);

    arrayInsert(&arr, 40, 10000);
    arrayShow(&arr);

    int pos = -1;
    if ((pos = arrayFind(&arr, 10000)) != -1)
        printf("10000 pos: %d\n", pos);
    else
        printf("未找到 10000\n");


    if ((pos = arrayFind(&arr, 10001)) != -1)
        printf("10001 pos: %d\n", pos);
    else
        printf("未找到 10001\n");

    if ((pos = arrayFind(&arr, 11)) != -1)
        printf("11 pos: %d\n", pos);
    else
        printf("未找到 11\n");

    if ((pos = arrayFind(&arr, 22)) != -1)
        printf("22 pos: %d\n", pos);
    else
        printf("未找到 22\n");

    if ((pos = arrayFind(&arr, 88)) != -1)
        printf("88 pos: %d\n", pos);
    else
        printf("未找到 88\n");

    if ((pos = arrayFind(&arr, 33)) != -1)
        printf("33 pos: %d\n", pos);
    else
        printf("未找到 33\n");

    if ((pos = arrayFind(&arr, 67)) != -1)
        printf("67 pos: %d\n", pos);
    else
        printf("未找到 67\n");

	arrayFree(&arr);

	return 0;
}
