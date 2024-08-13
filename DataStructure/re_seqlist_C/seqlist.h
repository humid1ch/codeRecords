#pragma once

#include <stdio.h>

typedef struct Array {
    int* _data;
    size_t  _size;
    size_t _capacity;
} Array;

int arrayInit(Array* arr, size_t n);

int arrayFree(Array* arr);

int arrayPush(Array* arr, int n);

int arrayPop(Array* arr);

int arrayInsert(Array* arr, size_t pos, int n);

int arrayErase(Array* arr, size_t pos);

void arrayShow(const Array* arr);

int arrayFind(const Array* arr, int n);
