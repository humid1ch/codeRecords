#define _CRT_SECURE_NO_WARNINGS 1

//#include "Heap.h"
//
//int main()
//{
//	//Heap hp;
//	//HeapInit(&hp);
//	//HeapPush(&hp, 5);
//	//HeapPush(&hp, 9);
//	//HeapPush(&hp, 2);
//	//HeapPush(&hp, 6);
//	//HeapPush(&hp, 9);
//	//HeapPush(&hp, 1);
//	//HeapPush(&hp, 7);
//	//HeapPush(&hp, 4);
//	//HeapPush(&hp, 3);
//	//HeapPrint(&hp);
//	//printf("\n");
//
//	//HeapPop(&hp);
//	//HeapPrint(&hp);
//	//printf("\n");
//
//	//HeapPop(&hp);
//	//HeapPrint(&hp);
//	//printf("\n");
//
//	//HeapPop(&hp);
//	//HeapPrint(&hp);
//	//printf("\n");
//
//	 // HeapSort ��֤
//	/*int a[] = { 5, 2, 67, 78, 23, 67, 43, 6, 2, 6, 3, 22, 33};
//	HeapSort(a, 13);
//	for (int i = 0; i < 13; i++)
//	{
//		printf("%d ", a[i]);
//	}
//	printf("\n");*/
//
//	// Top-K ����
//	// �������ݣ�ȡǰ����  ����������  ���� ǧ�� ����
//
//	// �����ܲ��ó������ �ȷ���ȥ��ǰ��   ��Ϊ���������Ҫ�����ݴ����ڴ��У����������ݶ����ڴ���˵ѹ������
//	// ���Ժ���������һ���Ǵ����ļ��еģ������ļ�ֻ��˳���ȡ�����������ȡ	
//	// ����������ճ���ı��������  �δ�  ������ ���ַ��������������ݱ��� k �Σ���ǳ����鷳
//	// ��ǰ 20 �͵ñ��� 20�Σ���ǰ 50 �͵ñ��� 50��
//	// Top-K �㷨����Ϊ�������������� д������
//	// ����� ���������е�ǰ K ������
//	// �ͽ��ļ���ǰ K��λ�õ����� ����Ϊ K ���ڵ��С���ѣ�Ȼ���ٴ� K λ������������ �� С���ѵĸ� ���Ƚ� �ȸ��� ����� �����µ���
//	// ������ͷ��β����һ�� �ͱ�֤�� ��K���ڵ��С���� �е����ݣ����Ǻ��������е� ǰ K ����������
//	// ����� ���������е�ǰ K ����С�ģ�������ͬ��˼·���� ����Ѿ�����
//
//	// �� ��ǰ 10 ����������Ϊ����ģ�⣩
//	//int n = 10000;
//	//int* a = (int*)malloc(sizeof(int)*n);
//	//srand(time(0));
//	//for (size_t i = 0; i < n; ++i)
//	//{//	�� ����a �д�� С�� 1000000 �����ֵ
//	//	a[i] = rand() % 1000000;
//	//}
//	//// �������� 10 ���� 1000000 ���ֵ
//	//a[5] = 1000000 + 1;
//	//a[1231] = 1000000 + 2;
//	//a[531] = 1000000 + 3;
//	//a[5121] = 1000000 + 4;
//	//a[115] = 1000000 + 5;
//	//a[2305] = 1000000 + 6;
//	//a[99] = 1000000 + 7;
//	//a[76] = 1000000 + 8;
//	//a[423] = 1000000 + 9;
//	//a[0] = 1000000 + 1000;
//	//// Top-K ��� ��ģ�⣩
//	//PrintTopK(a, n, 10);
//
//	return 0;
//}

#include "BinaryTree.h"

int main()
{
	BTNode* root = creatBinaryTree();

	printf("�������:> ");
	prevOrder(root);
	printf("\n");

	printf("�������:> ");
	inOrder(root);
	printf("\n");

	printf("�������:> ");
	postOrder(root);
	printf("\n");

	// ���Լ��� �������ڵ����� ��ȫ�ֱ���������
	// ���ʹ��ȫ�ֱ���������ÿ�ε��ô˺���ǰ ����Ҫ��ȫ�ֱ�������
	/*NodeCount = 0;
	BTNodeSize(root);
	printf("NodeSize:> %d\n", NodeCount);*/

	// ���Լ��� �������ڵ����� ������˼�룩
	printf("BTNodeSize:> %d\n", BTNodeSize(root));

	// ���Լ��� Ҷ�ӽ�������
	printf("LeafSize:> %d\n", LeafSize(root));
	// ���Լ��� ��n��Ľڵ����� ����n����������ȣ�
	printf("LevelSize_6:> %d\n", LevelSize(root, 6));
	// ���Լ��� ��n��Ľڵ����� ����nδ����������ȣ�
	printf("LevelSize_2:> %d\n", LevelSize(root, 2));
	// ���Լ��� �����������
	printf("Depth:> %d\n", BinaryTreeDepth(root));

	return 0;
}