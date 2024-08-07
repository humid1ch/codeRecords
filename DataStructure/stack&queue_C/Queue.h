#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>	// �ж϶��� �Ƿ�Ϊ��

typedef int QDatetype;

typedef struct QueueNode
{
	QDatetype date;
	struct QueueNode* next;
}QNode;

typedef struct Queue
{
	QNode* head;			// ����ͷ
	QNode* tail;			// ����β
}Queue;

// ���г�ʼ��
void QueueInit(Queue* pq);
// ��������
void QueueDestroy(Queue *pq);
// �����
void QueuePush(Queue *pq, QDatetype x);
// ������
void QueuePop(Queue *pq);
// �����п�
bool QueueEmpty(Queue *pq);
// ���г���
size_t QueueSize(Queue *pq);
// ����ͷ ����
QDatetype QueueFront(Queue *pq);
// ����β ����
QDatetype QueueBack(Queue *pq);
