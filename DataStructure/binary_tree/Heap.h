#pragma once 

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int HPDataType;

typedef struct Heap
{
	HPDataType* data;
	size_t size;
	size_t capacity;
}Heap;

// �� ��ʼ��
void HeapInit(Heap* php);
// �� ����
void HeapDestroy(Heap* php);
// �� ��������
void HeapPush(Heap* php, HPDataType x);
// �� ɾ������
void HeapPop(Heap* php);
// �� ���ݴ�ӡ
void HeapPrint(Heap* php);
// �п�
bool HeapEmpty(Heap* php);
// �ѵĸ�������
HPDataType HeapTop(Heap* php);
// �Ѵ�С
size_t HeapSize(Heap* php);
// ���� ����
void HeapSort(int *a, size_t size);
// Top-K ���� ˼·
void PrintTopK(int* a, int n, int k);