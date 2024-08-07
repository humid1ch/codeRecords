#pragma once

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>	// �ж�ջ �Ƿ�Ϊ��

typedef int StDateType;

typedef struct Stack
{
	StDateType *data;
	int top;				// ջ��λ��
	int capacity;			// ջ������
}Stack;

// ջ�ĳ�ʼ��
void StackInit(Stack* pSt);
// ջ������
void StackDestroy(Stack* pSt);
// ѹջ
void StackPush(Stack* pSt, StDateType x);
// ��ջ
void StackPop(Stack* pSt);
// �п�
bool StackEmpty(Stack* pSt);
// ջ������
StDateType StackTop(Stack* pSt);