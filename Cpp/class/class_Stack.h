#pragma once
#include <iostream>
#include <cstdlib>
#include <cassert>
using namespace std;

class Stack 
{
public:
	// ջ�ĳ�ʼ��
	void StackInit()
	{
		_data = nullptr;
		_top = 0;			// ջ����ʼλ�ö��� (top = -1, ��++��ֵ; top = 0, �ȸ�ֵ��++)
		_capacity = 0;
	}
	// ջ������
	void StackDestroy()
	{
		free(_data);
		_data = nullptr;
		_top = _capacity = 0;
	}
	// ѹջ
	void StackPush(int x)
	{
		if (_top == _capacity)
		{
			int newCapacity = _capacity == 0 ? 4 : _capacity * 2;
			int* tmp = (int*)realloc(_data, newCapacity * sizeof(int));
			if (tmp == nullptr)
			{
				printf("realloc failed\n");
				exit(-1);
			}
			else
				_data = tmp;

			_capacity = newCapacity;
		}

		_data[_top++] = x;
	}
	// ��ջ
	void StackPop()
	{
		assert(_top > 0);

		--_top;
	}
	// �п�
	bool StackEmpty()
	{
		return _top == 0;			// top ���� 0 ջΪ�գ����� true; ������ 0 ������ false
	}
	// ջ������
	int StackTop()
	{
		return _data[_top - 1];
	}

private:
	int *_data;
	int _top;				// ջ��λ��
	int _capacity;			// ջ������
};