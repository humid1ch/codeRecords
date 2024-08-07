#define _CRT_SECURE_NO_WARNINGS 1

#include "Stack.h"

// ջ�ĳ�ʼ��
void StackInit(Stack* pSt)
{
	assert(pSt);

	pSt->data = NULL;
	pSt->top = 0;			// ջ����ʼλ�ö��� (top = -1, ��++��ֵ; top = 0, �ȸ�ֵ��++)
	pSt->capacity = 0;
}
// ջ������
void StackDestroy(Stack* pSt)
{
	assert(pSt);

	free(pSt->data);
	pSt->data = NULL;
	pSt->top = pSt->capacity = 0;
}
// ѹջ
void StackPush(Stack* pSt, StDateType x)
{
	assert(pSt);

	if (pSt->top == pSt->capacity)
	{
		int newCapacity = pSt->capacity == 0 ? 4 : pSt->capacity * 2;
		StDateType* tmp = (StDateType*)realloc(pSt->data, newCapacity * sizeof(StDateType));
		if (tmp == NULL)
		{
			printf("realloc failed\n");
			exit(-1);
		}
		else
			pSt->data = tmp;

		pSt->capacity = newCapacity;
	}

	pSt->data[pSt->top++] = x;
}
// ��ջ
void StackPop(Stack* pSt)
{
	assert(pSt);
	assert(pSt->top > 0);

	--pSt->top;
}
// �п�
bool StackEmpty(Stack* pSt)
{
	assert(pSt);

	return pSt->top == 0;			// top ���� 0 ջΪ�գ����� true; ������ 0 ������ false
}
// ջ������
StDateType StackTop(Stack* pSt)
{
	assert(pSt);

	return pSt->data[pSt->top - 1];
}