#define _CRT_SECURE_NO_WARNINGS 1

#include "List.h"

void Test1()
{
	ListNode* phead = ListInit();
	printf("ͷ�� 1 ��2 ��3 ��4 ����:>\n");
	ListPushFront(phead, 1);
	ListPushFront(phead, 2);
	ListPushFront(phead, 3);
	ListPushFront(phead, 4);
	ListPrint(phead);
	printf("β�� 5 ��6 ��7 ��8 ����:>\n");
	ListPushBack(phead, 5);
	ListPushBack(phead, 6);
	ListPushBack(phead, 7);
	ListPushBack(phead, 8);
	ListPrint(phead);
	printf("βɾ���� * 2\n");
	ListPopBack(phead);
	ListPopBack(phead);
	ListPrint(phead);
	printf("ͷɾ���� * 2\n");
	ListPopFront(phead);
	ListPopFront(phead);
	ListPrint(phead);
	printf("���Ҳ��ԣ�9\n");
	ListNode* FindNode1 = ListFind(phead, 9);
	printf("FindNode1 = %p\n", FindNode1);
	printf("���Ҳ��ԣ�1\n");
	ListNode* FindNode2 = ListFind(phead, 1);
	printf("FindNode2 = %p\n", FindNode2);

	printf("FindNode2 λ��(1 ����λ��)ǰ���� 10:>\n");
	ListInsert(FindNode2, 10);
	ListPrint(phead);
	printf("ɾ�� FindNode2 λ��(1 ����λ��) �ڵ�:>\n");
	ListErase(FindNode2);
	ListPrint(phead);
	printf("��������:>\n");
	printf("����ǰ phead ��ַ:%p\n", phead);
	ListDestroy(&phead);
	printf("���ٺ� phead ��ַ:%p\n", phead);
}

int main()
{
	Test1();

	return 0;
}