#define _CRT_SECURE_NO_WARNINGS 1
#include "SingleList.h"


// �����������ݽڵ�
SListNode* BuySLTNode(SLTDateType x)
{
	SListNode* newnode = (SListNode*)malloc(sizeof(SListNode));
	if (newnode == NULL)
	{
		printf("malloc fail\n");
		exit(-1);
	}
	else
	{
		newnode->date = x;
		newnode->next = NULL;
	}

	return newnode;
}

// ����Ĵ�ӡ
void SListPrint(SListNode* phead)
{
	SListNode* cur = phead;
	while (cur != NULL)
	{
		printf("%d->", cur->date);
		cur = cur->next;
	}
	printf("NULL\n");
}

// ����β��
void SListPushBack(SListNode **pphead, SLTDateType x)
{
	assert(pphead);

	SListNode* newnode = BuySLTNode(x);
	if (*pphead == NULL)
	{// ����ͷ�ڵ�Ϊ�� ��������Ϊ�գ�
		*pphead = newnode;
	}
	else
	{// ����Ϊ��
		//��β
		SListNode* tail = *pphead;
		while (tail->next != NULL)
		{//ѭ�� �ҵ� tail->next Ϊ NULL
			tail = tail->next;
		}
		// �ҵ� tail->next Ϊ NULL, ���ҵ� ԭ�������β���ٰ�βָ�������ݽڵ�
		tail->next = newnode;
	}
}

// ������ͷ��
void SListPushFront(SListNode **pphead, SLTDateType x)
{
	assert(pphead);

	SListNode* newnode = BuySLTNode(x);
	newnode->next = *pphead;			//pphead ����ָ�룬�� &phead
	*pphead = newnode;
}

// ������βɾ
void SListPopBack(SListNode **pphead)
{
	assert(pphead);

	// 1.û�нڵ�
	// 2.ֻ��һ���ڵ�
	// 3.����ڵ�
	if (*pphead == NULL)
	{
		return;
	}
	else if ((*pphead)->next == NULL)
	{
		free(*pphead);
		*pphead = NULL;
	}
	else
	{
		SListNode *prev = NULL;
		SListNode *cur = *pphead;
		while (cur->next != NULL)
		{
			prev = cur;
			cur = cur->next;
		}

		free(cur);
		cur = NULL;
		prev->next = NULL;
	}
}
// ������ͷɾ
void SListPopFront(SListNode **pphead)
{
	assert(pphead);

	if (*pphead == NULL)
	{
		return;
	}
	else
	{
		SListNode* tail = (*pphead)->next;
		free(*pphead);
		*pphead = tail;
	}
}

// ���� x ����λ�� pos
SListNode* SListFind(SListNode *phead, SLTDateType x)
{
	SListNode *cur = phead;
	while (cur != NULL)
	{
		if (cur->date != x)
			cur = cur->next;
		else
			return cur;
		/*if (cur->date == x)
		return cur;

		cur = cur->next;*/
	}

	return NULL;
}

// ��posλ��֮ǰ����
void SListInsert(SListNode **pphead, SListNode *pos, SLTDateType x)
{
	assert(pphead);
	assert(pos);

	if (pos == *pphead)
	{ // pos�������е�һ���ڵ�  ֱ��ͷ��
		SListPushFront(pphead, x);
	}
	else
	{
		SListNode *cur = *pphead;
		while (cur->next != pos)
		{
			cur = cur->next;
		}

		SListNode *newNode = BuySLTNode(x);
		newNode->next = pos;
		cur->next = newNode;
	}
}

// ��posλ��֮�����
void SListInsertAfter(SListNode *pos, SLTDateType x)
{// ��Ϊ ֱ�ӽ��½ڵ����ӵ� pos �ڵ�֮�����Բ���Ҫԭ��������׽ڵ�
	assert(pos);

	SListNode *newNode = BuySLTNode(x);
	newNode->next = pos->next;
	pos->next = newNode;
}

// ɾ��posλ�ýڵ�
void SListErase(SListNode **pphead, SListNode *pos)
{
	assert(pphead);
	assert(pos);

	if (pos == *pphead)
	{
		SListPopFront(pphead);
	}
	else
	{
		SListNode *cur = *pphead;
		while (cur->next != pos)
		{
			cur = cur->next;
		}
		cur->next = pos->next;
		free(pos);
		pos = NULL;
	}
}

// ɾ��posλ��֮��ڵ�
void SListEraseAfter(SListNode *pos)
{
	assert(pos);

	SListNode* next = pos->next;
	if (next)
	{
		pos->next = next->next;
		free(next);
		next = NULL;
	}
}

// ���ٵ�����
void SListDestroy(SListNode **pphead)
{
	assert(pphead);
	// Ҫ����� ��free��ǰ�ڵ��ʱ�� ��Ҫ����ǰ�ڵ�� next �ڵ� ���������Ա�֤�����ҵ�
	SListNode *cur = *pphead;
	while (cur != NULL)
	{
		SListNode *next = cur->next;
		free(cur);
		cur = next;
	}

	*pphead = NULL;
}

