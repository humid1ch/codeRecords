#include "seqlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(int* x, int* y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

int arrayInit(Array* arr, size_t n) {
	arr->_data = NULL;
	int* tmp = (int*)malloc(sizeof(int) * n);
	if (tmp == NULL) {
		printf("Array init faild!\n");
		return -1;
	}

	arr->_data = tmp;
	arr->_size = 0;
	arr->_capacity = n;

	return 0;
}

int arrayFree(Array* arr) {
	if (arr->_data != NULL) {
		free(arr->_data);
	}
	arr->_data = NULL;
	arr->_size = 0;
	arr->_capacity = 0;

	return 0;
}

int arrayPush(Array* arr, int n) {
	if (arr->_size == arr->_capacity) {
		// 先扩容
		int newCapacity = arr->_capacity * 1.5;
		int* tmp = realloc(arr->_data, sizeof(int) * newCapacity);
		if (tmp == NULL) {
			printf("尾插数据失败\n");
			return -1;
		}

		arr->_data = tmp;
		arr->_capacity = newCapacity;
	}

	// 再赋值
	arr->_data[arr->_size] = n;
	arr->_size++;

	return 0;
}

int arrayPop(Array* arr) {
	if (arr == NULL || arr->_data == NULL || arr->_size <= 0) {
		printf("无数据\n");
		return -1;
	}

	arr->_size--;

	return 0;
}

int arrayInsert(Array* arr, size_t pos, int n) {
	if (arr == NULL || arr->_data == NULL || pos > arr->_capacity || pos < 0) {
		printf("插入失败\n");
		return -1;
	}

	if (arr->_size == arr->_capacity) {
		// 先扩容
		int newCapacity = arr->_capacity * 1.5;
		int* tmp = realloc(arr->_data, sizeof(int) * newCapacity);
		if (tmp == NULL) {
			printf("插入失败\n");
			return -1;
		}

		arr->_data = tmp;
		arr->_capacity = newCapacity;
	}

    arr->_size++;

    for (int i = arr->_size - 1; i >= 0; i--) {
        arr->_data[i + 1] = arr->_data[i];
        if (i == pos) {
            arr->_data[pos]  = n;
            break;
        }
    }

    if (pos > arr->_size) {
        arr->_size = pos;
    }

    return 0;
}

int arrayErase(Array* arr, size_t pos) {
	if (arr == NULL || arr->_data == NULL || arr->_size <= 0 || pos > arr->_capacity || pos < 0) {
		printf("删除失败\n");
		return -1;
	}

    swap(arr->_data + arr->_size - 1, arr->_data + pos);

    arr->_size--;

    return 0;
}

void arrayShow(const Array *arr) {
    if (arr == NULL || arr->_data == NULL) {
        printf("faild\n");
        return;
    }

	printf("arr capacity: %lu, size: %lu\n", arr->_capacity, arr->_size);
    for (int i = 0; i < arr->_size; i++) {
        printf("%d ", arr->_data[i]);
    }
    printf("\n");
    printf("\n");
}

int arrayFind(const Array* arr, int n) {
    if (arr == NULL || arr->_data == NULL) {
        return -1;
    }
    
    for (int i = 0; i < arr->_size; i++) {
        if (n == arr->_data[i]) {
            return i;   
        }
    }

    return -1;
}
