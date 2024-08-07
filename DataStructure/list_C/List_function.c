#define _CRT_SECURE_NO_WARNINGS 1

#include "List.h"

// ˫���ͷѭ������ڵ�
ListNode* BuyListNode(LTDataType x)
{
	ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
	if (newNode != NULL)
	{
		newNode->data = x;
		newNode->next = NULL;
		newNode->prev = NULL;
	}
	else
	{
		printf("BuyListNode failed!\n");
	}

	return newNode;
}
// ˫���ͷѭ�������ʼ��
ListNode* ListInit()
{
	// ˫���ͷѭ������ ��ʼ����Ҫ��ͷ���� next �� prev ��ָ���Լ� �Ա�֤��ѭ����
	ListNode* phead = BuyListNode(0);
	phead->next = phead;
	phead->prev = phead;

	return phead;
}
//void ListInit(ListNode** pphead)
//{
//	assert(pphead);
//
//	*pphead = BuyListNode(0);
//	(*pphead)->next = *pphead;
//	(*pphead)->prev = *pphead;
//}

// ˫���ͷѭ���������
void ListPrint(ListNode* phead)
{
	assert(phead);

	ListNode* cur = phead->next;	
	while (cur != phead)	
	{
		printf("%d->", cur->data);
		cur = cur->next;
	}
	printf("NULL\n");
}

// ˫���ͷѭ������β��
void ListPushBack(ListNode* phead, LTDataType x)
{
	assert(phead);

	/*ListNode* newNode = BuyListNode(x);
	ListNode* Prev = phead->prev;

	newNode->prev = Prev;
	newNode->next = phead;
	Prev->next = newNode;
	phead->prev = newNode;*/

	ListInsert(phead, x);
}

// ˫���ͷѭ������βɾ
void ListPopBack(ListNode* phead)
{
	assert(phead);
	assert(phead->next != phead);	//����������һ�������ݽڵ�

	/*ListNode* Prev = phead->prev;	//ͷ����ǰһ�ڵ� ��β�ڵ㣩
	ListNode* Prevv = Prev->prev;	//β�ڵ��ǰһ�ڵ�

	Prevv->next = phead;
	phead->prev = Prevv;
	
	free(Prev);
	Prev = NULL;*/

	ListErase(phead->prev);
}

// ˫���ͷѭ������ͷ��
void ListPushFront(ListNode* phead, LTDataType x)
{
	assert(phead);

	/*ListNode* newNode = BuyListNode(x);
	ListNode* next = phead->next;

	newNode->next = next;
	newNode->prev = phead;
	phead->next = newNode;
	next->prev = newNode;*/

	ListInsert(phead->next, x);
}

// ˫���ͷѭ������ͷɾ
void ListPopFront(ListNode* phead)
{
	assert(phead);
	assert(phead->next != phead);	//����������һ�������ݽڵ�

	/*ListNode* tail = phead->next;
	ListNode* tailNext = tail->next;

	phead->next = tailNext;
	tailNext->prev = phead;

	free(tail);
	tail = NULL;*/

	ListErase(phead->next);
}

// ˫���ͷѭ������ڵ����
ListNode* ListFind(ListNode* phead, LTDataType x)
{
	assert(phead);

	ListNode* cur = phead->next;
	while (cur != phead)
	{
		if (cur->data == x)
		{
			return cur;
		}

		cur = cur->next;
	}

	return NULL;
}

// ��pos֮ǰ����
void ListInsert(ListNode* pos, LTDataType x)
{
	assert(pos);

	ListNode* newNode = BuyListNode(x);
	ListNode* Prev = pos->prev;

	newNode->prev = Prev;
	newNode->next = pos;
	pos->prev = newNode;
	Prev->next = newNode;
}

// ɾ��posλ�õĽڵ�
void ListErase(ListNode* pos)
{
	assert(pos);
	// ������ʵҲ��Ҫ���� pos ������ ������ڱ�λ�ڵ�

	ListNode* posNext = pos->next;
	ListNode* posPrev = pos->prev;

	posNext->prev = posPrev;
	posPrev->next = posNext;

	free(pos);
	pos = NULL;
}

// ���ٵ�����
void ListDestroy(ListNode** pphead)
{
	assert(pphead);
	assert(*pphead);

	ListNode* cur = (*pphead)->next;
	while (cur != *pphead)
	{
		ListNode* next = cur->next;
		free(cur);
		// ListErase(cur);
		cur = next;
	}

	free(*pphead);
	*pphead = NULL;
}