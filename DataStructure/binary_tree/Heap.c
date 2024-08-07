#define _CRT_SECURE_NO_WARNINGS 1

#include "Heap.h"

// 堆的打印
void HeapPrint(Heap* php)
{
	assert(php);

	for (size_t i = 0; i < php->size; i++)
	{
		printf("%d ", php->data[i]);
	}
}

// 堆 初始化
void HeapInit(Heap* php)
{
	assert(php);

	php->data = NULL;
	php->capacity = php->size = 0;
}
// 堆 销毁
void HeapDestroy(Heap* php)
{
	assert(php);

	free(php->data);
	php->data = NULL;
	php->capacity = php->size = 0;
}
// 堆中数据交换
void swap(HPDataType* px, HPDataType* py)
{
	HPDataType tmp = *px;
	*px = *py;
	*py = tmp;
}

// 堆向上调整（确保大根堆）
void AdjustUp(HPDataType* data, size_t child)
{
	assert(data);

	// 孩子节点比父亲节点大 就交换位置
	while (child > 0)
	{
		size_t parent = (child - 1) / 2;
		if (data[child] > data[parent])
	  //if (data[child] < data[parent]) 就是向上调整保持小根堆
		{
			swap(&data[child], &data[parent]);
			child = parent;
		}
		else
		{
			break;
		}
	}

}
// 数据插入 （大根堆）
void HeapPush(Heap* php, HPDataType x)
{
	assert(php);

	if (php->size == php->capacity)
	{
		HPDataType newCapacity = php->capacity == 0 ? 4 : php->capacity * 2;
		HPDataType* tmp = (HPDataType*)realloc(php->data, sizeof(HPDataType)* newCapacity);
		if (tmp == NULL)
		{
			printf("realloc failed!\n");
			exit(-1);
		}

		php->data = tmp;
		php->capacity = newCapacity;
	}

	php->data[php->size] = x;
	++php->size;

	AdjustUp(php->data, php->size - 1);
}


void AdjustDown(HPDataType* data, size_t size, size_t root)
{
	assert(data);
	// 向下调整，需要对比父亲节点和孩子节点的大小
	// 如果父亲节点小于孩子节点就需要交换位置
	// 但是父亲节点一般情况下会有两个孩子节点  就需要判断出两个孩子节点中 更大的那个孩子节点 与 父亲节点进行交换
	// 为了方便判断 我们直接假设 左孩子节点就是大的孩子节点
	// 如果左孩子节点不是大的孩子节点  再让右孩子结点设置为大的孩子节点 
	// 然后再判断 父亲节点和这个大的孩子节点的大小
	// 然后再进行交换

	// 循环结束的条件是 ：child > size   当孩子节点位置大于实际拥有节点数量时停止
	size_t parent = root;
	size_t child = parent * 2 + 1; 
	while (child < size)
	{
		// 判断 大的孩子节点是否正确
		// 首先有 右孩子，然后才能判断左右孩子的大小，所以得先判断 child + 1 < size
		if (child + 1 < size && data[child] < data[child + 1])
		{
			++child;
		}

		// 父亲节点大 就 交换
		if (data[parent] < data[child])
		{
			swap(&data[parent], &data[child]);
			parent = child;
			child = parent * 2 + 1;
		}
		else
		{
			break;
		}
	}
}

// 数据删除（大根堆）
void HeapPop(Heap* php)
{//从堆的根删除，即每次删除 删除的是根
	assert(php);
	assert(php->size > 0);
	// 换一种思路，不用直接删除根
	// 将根 和 最后一个叶子节点交换位置 然后 size--，使原根节点数据从堆中消失
	// 此时 堆不是大根堆，因为最后一个叶子节点被交换到了根的位置，所以需要把此时的根节点向下调整至合适位置

	swap(&php->data[0], &php->data[php->size - 1]);
	php->size--;

	AdjustDown(php->data, php->size, 0);
}

// 判空
bool HeapEmpty(Heap* php)
{
	assert(php);

	return php->size == 0;
}

// 堆的根的数据
HPDataType HeapTop(Heap* php)
{
	assert(php);
	assert(php->size > 0);

	return php->data[0];
}

// 堆大小
size_t HeapSize(Heap* php)
{
	assert(php);

	return php->size;
}

//// 排序
//void HeapSort(int *data, size_t size)
//{
//	assert(data);
//
//	Heap hp;
//	HeapInit(&hp);
//	for (int i = 0; i < size; i++)
//	{
//		HeapPush(&hp, data[i]);		// 大根堆就降序，小根堆就升序
//	}
//
//	size_t j = 0;
//	while (!HeapEmpty(&hp))
//	{
//		data[j++] = HeapTop(&hp);
//		HeapPop(&hp);
//	}
//
//	HeapDestroy(&hp);
//}

// 排序  时间复杂度 O(N*log N)
//		空间复杂度 O(1)
void HeapSort(int *data, size_t size)
{// 直接对 数组建堆
	assert(data);

	// 向上调整建堆
	/*for (int i = 0; i < size; i++)
	{
		AdjustUp(data, i);
	}*/

	// 向下调整建堆
	// 两种调整方式都有一个共同的前提：除了需要调整的那个节点，二叉树已经是堆
	// 所以向下调整不能从 整个二叉树的root 开始
	// 要从 最后一个非叶子节点开始向下调整 （为什么是最后一个非叶子节点：因为叶子节点没有孩子节点不能再向下调整了）
	// 最后一个非叶子节点的位置是，最后一个节点的父亲节点

	int j = (size - 1 - 1) / 2;	//最后一个非叶子结点的位置
	for (j; j >= 0; j--)
	{
		AdjustDown(data, size, j);
	}

	// 大根堆建好，排序
	// 可以用 堆删除数据的思想
	// 将大根堆的 根 与 最后一个节点 交换位置
	// 然后从根向下调整
	// 然后 再将根 与 倒数第二个节点 交换位置
	// 然后从根向下调整  以此类推
	size_t end = size - 1;	 // end 先从最后一个节点位置开始
	while (end > 0)
	{
		swap(&data[0], &data[end]);	// 根节点 与 end节点 交换位置
		AdjustDown(data, end, 0);	// 从根节点向下调整 调整到 最后结束
		--end;						// end 位置 减一
	}
	
}

void PrintTopK(int* data, int size, int k)
{
	// 建立小根堆
	Heap hp;
	HeapInit(&hp);
	for (int i = 0; i < k; i++)
	{
		HeapPush(&hp, data[i]);
	}

	for (int j = k; j < size; j++)
	{
		if (data[j] >(&hp)->data[0])
		{//如果 当前数据 比 根的数据 大，就删除根 并在堆中放入当前数据
			HeapPop(&hp);
			HeapPush(&hp, data[j]);
		}
	}

	HeapPrint(&hp);
	printf("\n");
}