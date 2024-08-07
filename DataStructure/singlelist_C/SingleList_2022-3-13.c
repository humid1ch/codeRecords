#define _CRT_SECURE_NO_WARNINGS 1

#include "SingleList.h"

void test1()
{
	SListNode *phead = NULL;
	SListPushFront(&phead, 1);
	SListPushFront(&phead, 2);
	SListPushFront(&phead, 3);
	SListPushFront(&phead, 4);
	SListPushFront(&phead, 5);
	SListPrint(phead);
	SListNode *pos = SListFind(phead, 7);
	printf("%p", pos);
}

void test2()
{
	SListNode *phead = NULL;
	SListPushFront(&phead, 1);
	SListPushFront(&phead, 2);
	SListPushFront(&phead, 3);
	SListPushFront(&phead, 4);
	SListPushFront(&phead, 5);
	SListPrint(phead);
	SListPopBack(&phead);
	SListPopBack(&phead);
	SListPopBack(&phead);
	SListPopBack(&phead);
	SListPrint(phead);
	SListPopBack(&phead);
	SListPrint(phead);
}

void test3()
{
	SListNode *slist = NULL;
	printf("ͷ��β�����:>\n");
	SListPushFront(&slist, 1);
	SListPushFront(&slist, 2);
	SListPushFront(&slist, 3);
	SListPushBack(&slist, 4);
	SListPushBack(&slist, 5);
	SListPushBack(&slist, 6);
	SListPrint(slist);
	printf("\n");
	printf("βɾ����:>\n");
	SListPopBack(&slist);
	SListPrint(slist);
	printf("\n");
	printf("ͷɾ����:>\n");
	SListPopFront(&slist);
	SListPrint(slist);
	printf("\n");
	printf("posλ��֮ǰ�������(pos Ϊ�������� 1 ��λ��):>\n");
	SListNode *pos1 = SListFind(slist, 1);
	if (pos1 != NULL)
	{
		SListInsert(&slist, pos1, 10);
	}
	SListPrint(slist);
	printf("\n");

	printf("posλ��֮��������(pos Ϊ�������� 4 ��λ��):>\n");
	SListNode *pos2 = SListFind(slist, 4);
	if (pos2 != NULL)
	{
		SListInsertAfter(pos2, 10);
	}
	SListPrint(slist);
	printf("\n");

	printf("posλ��ɾ������(pos Ϊ�������� 4 ��λ��):>\n");
	SListNode *pos3 = SListFind(slist, 4);
	if (pos3 != NULL)
	{
		SListErase(&slist, pos3);
	}
	SListPrint(slist);
	printf("\n");

	printf("posλ��֮��ɾ������(pos Ϊ�������� 10 ��λ��):>\n");
	SListNode *pos4 = SListFind(slist, 10);
	if (pos4 != NULL)
	{
		SListEraseAfter(pos4);
	}
	SListPrint(slist);
	printf("\n");

	printf("���������ٲ���:>\n");
	SListDestroy(&slist);
	SListPrint(slist);
	printf("\n");
}

int main()
{
	test3();

	return 0;
}