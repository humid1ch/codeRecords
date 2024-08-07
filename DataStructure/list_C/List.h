#pragma once

#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <stdlib.h>

typedef int LTDataType;

typedef struct ListNode
{
	LTDataType data;
	struct ListNode *next;
	struct ListNode *prev;
}ListNode;


// ˫���ͷѭ������ڵ�
ListNode* BuyListNode(LTDataType x);

// ˫���ͷѭ�������ʼ��
ListNode* ListInit();
//void ListInit(ListNode** pphead);

// ˫���ͷѭ���������
void ListPrint(ListNode* phead);

// ˫���ͷѭ������β��
void ListPushBack(ListNode* phead, LTDataType x);

// ˫���ͷѭ������βɾ
void ListPopBack(ListNode* phead);

// ˫���ͷѭ������ͷ��
void ListPushFront(ListNode* phead, LTDataType x);

// ˫���ͷѭ������ͷɾ
void ListPopFront(ListNode* phead);

// ˫���ͷѭ������ڵ����
ListNode* ListFind(ListNode* phead, LTDataType x);

// ��pos֮ǰ����
void ListInsert(ListNode* pos, LTDataType x);

// ɾ��posλ�õĽڵ�
void ListErase(ListNode* pos);

// ���ٵ�����
void ListDestroy(ListNode** phead);

