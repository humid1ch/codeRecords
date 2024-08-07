#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef int SLTDateType;

typedef struct SListNode
{
	SLTDateType date;
	struct SListNode *next;
}SListNode; 

// ����Ĵ�ӡ
void SListPrint(SListNode *phead);

// ����β��
void SListPushBack(SListNode **pphead, SLTDateType x);

// ������ͷ��
void SListPushFront(SListNode **pphead, SLTDateType x);

// ������ͷɾ
void SListPopFront(SListNode **pphead);

// ������βɾ
void SListPopBack(SListNode **pphead);

// ���� x ����λ�� pos
SListNode* SListFind(SListNode *phead, SLTDateType x);

// ��posλ��֮ǰ����
void SListInsert(SListNode **pphead, SListNode *pos, SLTDateType x);

// ��posλ��֮�����
void SListInsertAfter(SListNode *pos, SLTDateType x);

// ɾ��posλ�ýڵ�
void SListErase(SListNode **pphead, SListNode *pos);

// ɾ��posλ��֮��ڵ�
void SListEraseAfter(SListNode *pos);

// ���ٵ�����
void SListDestroy(SListNode **pphead);