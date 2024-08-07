#define _CRT_SECURE_NO_WARNINGS 1

#include "Queue.h"

// ���г�ʼ��
void QueueInit(Queue* pq)
{
	assert(pq);

	pq->head = pq->tail = NULL;		// ����ͷ�� ����β ͬʱ�� NULL
}
// ��������
void QueueDestroy(Queue *pq)
{
	assert(pq);

	QNode* cur = pq->head;
	while (cur)						// pq->tail->next ��Ϊ NULL
	{
		QNode* next = cur->next;
		free(cur);
		cur = next;
	}

	pq->head = pq->tail = NULL;		// �����������ٺ�ͷβָ�� �ÿ�
}
// �����
void QueuePush(Queue *pq, QDatetype x)
{
	assert(pq);

	QNode* tmp = (QNode*)malloc(sizeof(QNode));
	if (tmp == NULL)
	{
		printf("malloc failed\n");
		return ;
	}
	QNode* newNode = tmp;
	newNode->date = x;
	newNode->next = NULL;

	if (pq->head == NULL)
	{
		pq->head = pq->tail = newNode;
	}
	else
	{
		pq->tail->next = newNode;
		pq->tail = newNode;
	}
}
// ������
void QueuePop(Queue *pq)
{
	assert(pq);
	assert(pq->head != NULL && pq->tail != NULL);

	if (pq->head->next == NULL)
	{
		free(pq->head);
		pq->head = pq->tail = NULL;
	}
	else
	{
		QNode* HeadNext = pq->head->next;
		free(pq->head);
		pq->head = HeadNext;
	}
}
// �����п�
bool QueueEmpty(Queue *pq)
{
	assert(pq);

	return pq->head == NULL;
}
// ���г���
size_t QueueSize(Queue *pq)
{
	assert(pq);

	QNode* cur = pq->head;
	size_t size = 0;
	while (cur)
	{
		++size;
		cur = cur->next;
	}

	return size;
}
// ����ͷ ����
QDatetype QueueFront(Queue *pq)
{
	assert(pq);
	assert(pq->head);

	return pq->head->date;
}
// ����β ����
QDatetype QueueBack(Queue *pq)
{
	assert(pq);
	assert(pq->tail);

	return pq->tail->date;
}
