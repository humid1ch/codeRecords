#define _CRT_SECURE_NO_WARNINGS 1

#include "SeqList.h"

// ˳����������
void SeqListCheckCapacity(SeqList *psl)
{
	assert(psl);

	if (psl->size == psl->capacity)
	{
		size_t newCapacity = psl->capacity == 0 ? 4 : psl->capacity * 2;
		SLDateType *tmp = (SLDateType*)realloc(psl->s, sizeof(SLDateType) * newCapacity);
		if (tmp != NULL)
		{
			psl->s = tmp;
			psl->capacity = newCapacity;
		}
		else
		{
			printf("realloc failed\n");
		}
	}
}

// ˳����ʼ��
void SeqListInit(SeqList *psl)
{
	assert(psl);

	psl->s = NULL;
	psl->size = 0;
	psl->capacity = 0;
}

// ˳���β��
void SeqListPushBack(SeqList *psl, SLDateType x)
{
	assert(psl);

	/*SeqListCheckCapacity(psl);

	psl->s[psl->size] = x;
	psl->size++;*/

	SeqListInsert(psl, psl->size, x);		//ֱ�ӵ��� ˳������ ����λ��Ϊ psl->size
}

// ˳���βɾ
void SeqListPopBack(SeqList *psl)
{
	assert(psl);
	
	//if (psl->size > 0)		// ˳�����Ҫ������
	//{
	//	psl->size--;
	//}

	SeqListErase(psl, psl->size - 1);		// ֱ�ӵ��� pos λ��ɾ��
}

// ˳����ӡ
void SeqListPrint(SeqList *psl)
{
	assert(psl);

	for (int i = 0; i < psl->size; i++)
	{
		printf("%d ", psl->s[i]);
	}
	printf("\n");
}

// ˳���ǰ��
void SeqListPushFront(SeqList *psl, SLDateType x)
{
	assert(psl);

	//SeqListCheckCapacity(psl);

	//for (int i = psl->size - 1; i >= 0; i--)
	//{
	//	psl->s[i+1] = psl->s[i];
	//}
	///*
	//int end = psl->size - 1;
	//while(end >= 0)
	//{
	//	psl->s[end+1] = psl->s[end];
	//	end--;
	//}
	//*/

	//psl->s[0] = x;
	//psl->size++;

	SeqListInsert(psl, 0, x);			// ֱ�ӵ��� pos λ�ò��뺯��
}

// ˳���ǰɾ
void SeqListPopFront(SeqList *psl)
{
	assert(psl);

	//if (psl->size > 0)
	//{
	//	for (int i = 1; i < psl->size; i++)
	//	{
	//		psl->s[i - 1] = psl->s[i];
	//	}
	//	/*
	//	int begin = 1;
	//	while( begin < psl->size)
	//	{
	//		psl->s[begin - 1] = psl->s[begin];
	//		begin++;
	//	}
	//	*/

	//	psl->size--;
	//}

	SeqListErase(psl, 0);			// ֱ�ӵ��� pos λ��ɾ������
}

// ˳������(��posλ�ò���x)
void SeqListInsert(SeqList *psl, size_t pos, SLDateType x)
{
	assert(psl);

	//assert(pos < psl->size);
	if (pos > psl->size)
	{// ��� pos �Ƿ�Խ��
		printf("pos Խ�磺pos = %d", pos);
		return;
	}

	SeqListCheckCapacity(psl);

	/*int end = psl->size - 1;
	while (end >= (int)pos)				//pos ǿתΪ int���ͣ���ֹ psl->size === 0ʱ��end === -1 �ڱȽϵ�ʱ����������Ϊ size_t ���͡�
	{
		psl->s[end] = psl->s[end + 1];
		end--;
	}*/
	size_t end = psl->size;				//���ַ���Ҳ���Է�ֹ �������������ֱ�ӷ�ֹ end �����踺������Ϊ psl->size ����� 0 
	while (end > pos)
	{
		psl->s[end] = psl->s[end - 1];
		end--;
	}
	
	psl->s[pos] = x;
	psl->size++;
}

// ɾ�� pos λ�õ�����
void SeqListErase(SeqList *psl, size_t pos)
{
	assert(psl);

	//assert(pos < psl->size);
	if (pos >= psl->size)
	{// ��� pos �Ƿ�Խ��
		printf("pos Խ�磺pos = %d", pos);
		return;
	}

	size_t begin = pos + 1;
	while (begin < psl->size)
	{
		psl->s[begin - 1] = psl->s[begin];
		begin++;
	}

	psl->size--;
}

// ˳������
int SeqListFind(SeqList *psl, SLDateType x)
{
	for (size_t i = 0; i < psl->size; i++)
	{
		if (psl->s[i] == x)
		{
			return i;
		}
	}

	return -1;
}

// ˳�������
void SeqListDestory(SeqList *psl)
{
	assert(psl);

	free(psl->s);
	psl->s = NULL;
	psl->size = 0;
	psl->capacity = 0;
}